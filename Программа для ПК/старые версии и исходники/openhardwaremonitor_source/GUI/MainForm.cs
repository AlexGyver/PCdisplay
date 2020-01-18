/*
 
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
  Copyright (C) 2009-2013 Michael Möller <mmoeller@openhardwaremonitor.org>
	Copyright (C) 2010 Paul Werelds <paul@werelds.net>
	Copyright (C) 2012 Prince Samuel <prince.samuel@gmail.com>

*/

using Aga.Controls.Tree;
using Aga.Controls.Tree.NodeControls;
using OpenHardwareMonitor.Hardware;
using OpenHardwareMonitor.Utilities;
using OpenHardwareMonitor.WMI;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows.Forms;

namespace OpenHardwareMonitor.GUI {
  public partial class MainForm : Form {

    public PersistentSettings settings;
    private UnitManager unitManager;
    private Computer computer;
    private Node root;
    private TreeModel treeModel;
    private IDictionary<ISensor, Color> sensorPlotColors =
      new Dictionary<ISensor, Color>();
    private Color[] plotColorPalette;
    private SystemTray systemTray;
    private StartupManager startupManager = new StartupManager();
    private UpdateVisitor updateVisitor = new UpdateVisitor();
    private SensorGadget gadget;
    private Form plotForm;
    private PlotPanel plotPanel;

    private UserOption showHiddenSensors;
    private UserOption showPlot;
    private UserOption showValue;
    private UserOption showMin;
    private UserOption showMax;
    private UserOption startMinimized;
    private UserOption minimizeToTray;
    private UserOption minimizeOnClose;
    private UserOption autoStart;

    private UserOption readMainboardSensors;
    private UserOption readCpuSensors;
    private UserOption readRamSensors;
    private UserOption readGpuSensors;
    private UserOption readFanControllersSensors;
    private UserOption readHddSensors;

    private UserOption showGadget;
    private UserRadioGroup plotLocation;
    private WmiProvider wmiProvider;

    private UserOption runWebServer;
    private HttpServer server;

    private UserOption runSerial;
    private Serial serial;

    private UserOption logSensors;
    private UserRadioGroup loggingInterval;
    private Logger logger;

    private bool selectionDragging = false;

    public MainForm() {
      InitializeComponent();

      // check if the OpenHardwareMonitorLib assembly has the correct version
      if (Assembly.GetAssembly(typeof(Computer)).GetName().Version !=
        Assembly.GetExecutingAssembly().GetName().Version) {
        MessageBox.Show(
          "The version of the file OpenHardwareMonitorLib.dll is incompatible.",
          "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        Environment.Exit(0);
      }

      this.settings = new PersistentSettings();
      this.settings.Load(Path.ChangeExtension(
        Application.ExecutablePath, ".config"));

      this.unitManager = new UnitManager(settings);

      // make sure the buffers used for double buffering are not disposed 
      // after each draw call
      BufferedGraphicsManager.Current.MaximumBuffer =
        Screen.PrimaryScreen.Bounds.Size;

      // set the DockStyle here, to avoid conflicts with the MainMenu
      this.splitContainer.Dock = DockStyle.Fill;

      this.Font = SystemFonts.MessageBoxFont;
      treeView.Font = SystemFonts.MessageBoxFont;

      plotPanel = new PlotPanel(settings, unitManager) {
        Font = SystemFonts.MessageBoxFont,
        Dock = DockStyle.Fill
      };

      nodeCheckBox.IsVisibleValueNeeded += nodeCheckBox_IsVisibleValueNeeded;
      nodeTextBoxText.DrawText += nodeTextBoxText_DrawText;
      nodeTextBoxValue.DrawText += nodeTextBoxText_DrawText;
      nodeTextBoxMin.DrawText += nodeTextBoxText_DrawText;
      nodeTextBoxMax.DrawText += nodeTextBoxText_DrawText;
      nodeTextBoxText.EditorShowing += nodeTextBoxText_EditorShowing;

      foreach (TreeColumn column in treeView.Columns) {
        column.Width = Math.Max(20, Math.Min(400,
  settings.GetValue("treeView.Columns." + column.Header + ".Width",
  column.Width)));
      }

      treeModel = new TreeModel();
      root = new Node(System.Environment.MachineName) {
        Image = Utilities.EmbeddedResources.GetImage("computer.png")
      };

      treeModel.Nodes.Add(root);
      treeView.Model = treeModel;

      this.computer = new Computer(settings);

      systemTray = new SystemTray(computer, settings, unitManager);
      systemTray.HideShowCommand += hideShowClick;
      systemTray.ExitCommand += exitClick;

      int p = (int)Environment.OSVersion.Platform;
      if ((p == 4) || (p == 128)) { // Unix
        treeView.RowHeight = Math.Max(treeView.RowHeight, 18);
        splitContainer.BorderStyle = BorderStyle.None;
        splitContainer.Border3DStyle = Border3DStyle.Adjust;
        splitContainer.SplitterWidth = 4;
        treeView.BorderStyle = BorderStyle.Fixed3D;
        plotPanel.BorderStyle = BorderStyle.Fixed3D;
        gadgetMenuItem.Visible = false;
        minCloseMenuItem.Visible = false;
        minTrayMenuItem.Visible = false;
        startMinMenuItem.Visible = false;
      } else { // Windows
        treeView.RowHeight = Math.Max(treeView.Font.Height + 1, 18);

        gadget = new SensorGadget(computer, settings, unitManager);
        gadget.HideShowCommand += hideShowClick;

        wmiProvider = new WmiProvider(computer);
      }

      logger = new Logger(computer);

      plotColorPalette = new Color[13];
      plotColorPalette[0] = Color.Blue;
      plotColorPalette[1] = Color.OrangeRed;
      plotColorPalette[2] = Color.Green;
      plotColorPalette[3] = Color.LightSeaGreen;
      plotColorPalette[4] = Color.Goldenrod;
      plotColorPalette[5] = Color.DarkViolet;
      plotColorPalette[6] = Color.YellowGreen;
      plotColorPalette[7] = Color.SaddleBrown;
      plotColorPalette[8] = Color.RoyalBlue;
      plotColorPalette[9] = Color.DeepPink;
      plotColorPalette[10] = Color.MediumSeaGreen;
      plotColorPalette[11] = Color.Olive;
      plotColorPalette[12] = Color.Firebrick;

      computer.HardwareAdded += new HardwareEventHandler(HardwareAdded);
      computer.HardwareRemoved += new HardwareEventHandler(HardwareRemoved);

      computer.Open();

      timer.Enabled = true;

      showHiddenSensors = new UserOption("hiddenMenuItem", false,
        hiddenMenuItem, settings);
      showHiddenSensors.Changed += delegate (object sender, EventArgs e) {
        treeModel.ForceVisible = showHiddenSensors.Value;
      };

      showValue = new UserOption("valueMenuItem", true, valueMenuItem,
        settings);
      showValue.Changed += delegate (object sender, EventArgs e) {
        treeView.Columns[1].IsVisible = showValue.Value;
      };

      showMin = new UserOption("minMenuItem", false, minMenuItem, settings);
      showMin.Changed += delegate (object sender, EventArgs e) {
        treeView.Columns[2].IsVisible = showMin.Value;
      };

      showMax = new UserOption("maxMenuItem", true, maxMenuItem, settings);
      showMax.Changed += delegate (object sender, EventArgs e) {
        treeView.Columns[3].IsVisible = showMax.Value;
      };

      startMinimized = new UserOption("startMinMenuItem", false,
        startMinMenuItem, settings);

      minimizeToTray = new UserOption("minTrayMenuItem", true,
        minTrayMenuItem, settings);
      minimizeToTray.Changed += delegate (object sender, EventArgs e) {
        systemTray.IsMainIconEnabled = minimizeToTray.Value;
      };

      minimizeOnClose = new UserOption("minCloseMenuItem", false,
        minCloseMenuItem, settings);

      autoStart = new UserOption(null, startupManager.Startup,
        startupMenuItem, settings);
      autoStart.Changed += delegate (object sender, EventArgs e) {
        try {
          startupManager.Startup = autoStart.Value;
        } catch (InvalidOperationException) {
          MessageBox.Show("Updating the auto-startup option failed.", "Error",
            MessageBoxButtons.OK, MessageBoxIcon.Error);
          autoStart.Value = startupManager.Startup;
        }
      };

      readMainboardSensors = new UserOption("mainboardMenuItem", true,
        mainboardMenuItem, settings);
      readMainboardSensors.Changed += delegate (object sender, EventArgs e) {
        computer.MainboardEnabled = readMainboardSensors.Value;
      };

      readCpuSensors = new UserOption("cpuMenuItem", true,
        cpuMenuItem, settings);
      readCpuSensors.Changed += delegate (object sender, EventArgs e) {
        computer.CPUEnabled = readCpuSensors.Value;
      };

      readRamSensors = new UserOption("ramMenuItem", true,
        ramMenuItem, settings);
      readRamSensors.Changed += delegate (object sender, EventArgs e) {
        computer.RAMEnabled = readRamSensors.Value;
      };

      readGpuSensors = new UserOption("gpuMenuItem", true,
        gpuMenuItem, settings);
      readGpuSensors.Changed += delegate (object sender, EventArgs e) {
        computer.GPUEnabled = readGpuSensors.Value;
      };

      readFanControllersSensors = new UserOption("fanControllerMenuItem", true,
        fanControllerMenuItem, settings);
      readFanControllersSensors.Changed += delegate (object sender, EventArgs e) {
        computer.FanControllerEnabled = readFanControllersSensors.Value;
      };

      readHddSensors = new UserOption("hddMenuItem", true, hddMenuItem,
        settings);
      readHddSensors.Changed += delegate (object sender, EventArgs e) {
        computer.HDDEnabled = readHddSensors.Value;
      };

      showGadget = new UserOption("gadgetMenuItem", false, gadgetMenuItem,
        settings);
      showGadget.Changed += delegate (object sender, EventArgs e) {
        if (gadget != null) {
          gadget.Visible = showGadget.Value;
        }
      };

      celsiusMenuItem.Checked =
        unitManager.TemperatureUnit == TemperatureUnit.Celsius;
      fahrenheitMenuItem.Checked = !celsiusMenuItem.Checked;

      server = new HttpServer(root, this.settings.GetValue("listenerPort", 8085));
      if (server.PlatformNotSupported) {
        webMenuItemSeparator.Visible = false;
        webMenuItem.Visible = false;
      }

      runWebServer = new UserOption("runWebServerMenuItem", false,
        runWebServerMenuItem, settings);
      runWebServer.Changed += delegate (object sender, EventArgs e) {
        if (runWebServer.Value) {
          server.StartHTTPListener();
        } else {
          server.StopHTTPListener();
        }
      };

      serial = new Serial();
      runSerial = new UserOption("runSerialMenuItem", false, runSerialMenuItem, settings);
      runSerial.Changed += delegate (object sender, EventArgs e) {
        if (runSerial.Value) {
          serial.Open();
        } else {
          serial.Close();
        }
      };

      logSensors = new UserOption("logSensorsMenuItem", false, logSensorsMenuItem,
        settings);

      loggingInterval = new UserRadioGroup("loggingInterval", 0,
        new[] { log1sMenuItem, log2sMenuItem, log5sMenuItem, log10sMenuItem,
        log30sMenuItem, log1minMenuItem, log2minMenuItem, log5minMenuItem,
        log10minMenuItem, log30minMenuItem, log1hMenuItem, log2hMenuItem,
        log6hMenuItem},
        settings);
      loggingInterval.Changed += (sender, e) => {
        switch (loggingInterval.Value) {
          case 0: logger.LoggingInterval = new TimeSpan(0, 0, 1); break;
          case 1: logger.LoggingInterval = new TimeSpan(0, 0, 2); break;
          case 2: logger.LoggingInterval = new TimeSpan(0, 0, 5); break;
          case 3: logger.LoggingInterval = new TimeSpan(0, 0, 10); break;
          case 4: logger.LoggingInterval = new TimeSpan(0, 0, 30); break;
          case 5: logger.LoggingInterval = new TimeSpan(0, 1, 0); break;
          case 6: logger.LoggingInterval = new TimeSpan(0, 2, 0); break;
          case 7: logger.LoggingInterval = new TimeSpan(0, 5, 0); break;
          case 8: logger.LoggingInterval = new TimeSpan(0, 10, 0); break;
          case 9: logger.LoggingInterval = new TimeSpan(0, 30, 0); break;
          case 10: logger.LoggingInterval = new TimeSpan(1, 0, 0); break;
          case 11: logger.LoggingInterval = new TimeSpan(2, 0, 0); break;
          case 12: logger.LoggingInterval = new TimeSpan(6, 0, 0); break;
        }
      };

      InitializePlotForm();

      startupMenuItem.Visible = startupManager.IsAvailable;

      if (startMinMenuItem.Checked) {
        if (!minTrayMenuItem.Checked) {
          WindowState = FormWindowState.Minimized;
          Show();
        }
      } else {
        Show();
      }

      // Create a handle, otherwise calling Close() does not fire FormClosed     
      IntPtr handle = Handle;

      // Make sure the settings are saved when the user logs off
      Microsoft.Win32.SystemEvents.SessionEnded += delegate {
        computer.Close();
        SaveConfiguration();
        if (runWebServer.Value) {
          server.Quit();
        }
      };
    }

    private void InitializePlotForm() {
      plotForm = new Form {
        FormBorderStyle = FormBorderStyle.SizableToolWindow,
        ShowInTaskbar = false,
        StartPosition = FormStartPosition.Manual
      };
      this.AddOwnedForm(plotForm);
      plotForm.Bounds = new Rectangle {
        X = settings.GetValue("plotForm.Location.X", -100000),
        Y = settings.GetValue("plotForm.Location.Y", 100),
        Width = settings.GetValue("plotForm.Width", 600),
        Height = settings.GetValue("plotForm.Height", 400)
      };

      showPlot = new UserOption("plotMenuItem", false, plotMenuItem, settings);
      plotLocation = new UserRadioGroup("plotLocation", 0,
        new[] { plotWindowMenuItem, plotBottomMenuItem, plotRightMenuItem },
        settings);

      showPlot.Changed += delegate (object sender, EventArgs e) {
        if (plotLocation.Value == 0) {
          if (showPlot.Value && this.Visible) {
            plotForm.Show();
          } else {
            plotForm.Hide();
          }
        } else {
          splitContainer.Panel2Collapsed = !showPlot.Value;
        }
        treeView.Invalidate();
      };
      plotLocation.Changed += delegate (object sender, EventArgs e) {
        switch (plotLocation.Value) {
          case 0:
            splitContainer.Panel2.Controls.Clear();
            splitContainer.Panel2Collapsed = true;
            plotForm.Controls.Add(plotPanel);
            if (showPlot.Value && this.Visible) {
              plotForm.Show();
            }

            break;
          case 1:
            plotForm.Controls.Clear();
            plotForm.Hide();
            splitContainer.Orientation = Orientation.Horizontal;
            splitContainer.Panel2.Controls.Add(plotPanel);
            splitContainer.Panel2Collapsed = !showPlot.Value;
            break;
          case 2:
            plotForm.Controls.Clear();
            plotForm.Hide();
            splitContainer.Orientation = Orientation.Vertical;
            splitContainer.Panel2.Controls.Add(plotPanel);
            splitContainer.Panel2Collapsed = !showPlot.Value;
            break;
        }
      };

      plotForm.FormClosing += delegate (object sender, FormClosingEventArgs e) {
        if (e.CloseReason == CloseReason.UserClosing) {
          // just switch off the plotting when the user closes the form
          if (plotLocation.Value == 0) {
            showPlot.Value = false;
          }
          e.Cancel = true;
        }
      };

      EventHandler moveOrResizePlotForm = delegate (object sender, EventArgs e) {
        if (plotForm.WindowState != FormWindowState.Minimized) {
          settings.SetValue("plotForm.Location.X", plotForm.Bounds.X);
          settings.SetValue("plotForm.Location.Y", plotForm.Bounds.Y);
          settings.SetValue("plotForm.Width", plotForm.Bounds.Width);
          settings.SetValue("plotForm.Height", plotForm.Bounds.Height);
        }
      };
      plotForm.Move += moveOrResizePlotForm;
      plotForm.Resize += moveOrResizePlotForm;

      plotForm.VisibleChanged += delegate (object sender, EventArgs e) {
        Rectangle bounds = new Rectangle(plotForm.Location, plotForm.Size);
        Screen screen = Screen.FromRectangle(bounds);
        Rectangle intersection =
          Rectangle.Intersect(screen.WorkingArea, bounds);
        if (intersection.Width < Math.Min(16, bounds.Width) ||
            intersection.Height < Math.Min(16, bounds.Height)) {
          plotForm.Location = new Point(
            screen.WorkingArea.Width / 2 - bounds.Width / 2,
            screen.WorkingArea.Height / 2 - bounds.Height / 2);
        }
      };

      this.VisibleChanged += delegate (object sender, EventArgs e) {
        if (this.Visible && showPlot.Value && plotLocation.Value == 0) {
          plotForm.Show();
        } else {
          plotForm.Hide();
        }
      };
    }

    private void InsertSorted(Collection<Node> nodes, HardwareNode node) {
      int i = 0;
      while (i < nodes.Count && nodes[i] is HardwareNode &&
        ((HardwareNode)nodes[i]).Hardware.HardwareType <
          node.Hardware.HardwareType) {
        i++;
      }

      nodes.Insert(i, node);
    }

    private void SubHardwareAdded(IHardware hardware, Node node) {
      HardwareNode hardwareNode =
        new HardwareNode(hardware, settings, unitManager);
      hardwareNode.PlotSelectionChanged += PlotSelectionChanged;

      InsertSorted(node.Nodes, hardwareNode);

      foreach (IHardware subHardware in hardware.SubHardware) {
        SubHardwareAdded(subHardware, hardwareNode);
      }
    }

    private void HardwareAdded(IHardware hardware) {
      SubHardwareAdded(hardware, root);
      PlotSelectionChanged(this, null);
    }

    private void HardwareRemoved(IHardware hardware) {
      List<HardwareNode> nodesToRemove = new List<HardwareNode>();
      foreach (Node node in root.Nodes) {
        HardwareNode hardwareNode = node as HardwareNode;
        if (hardwareNode != null && hardwareNode.Hardware == hardware) {
          nodesToRemove.Add(hardwareNode);
        }
      }
      foreach (HardwareNode hardwareNode in nodesToRemove) {
        root.Nodes.Remove(hardwareNode);
        hardwareNode.PlotSelectionChanged -= PlotSelectionChanged;
      }
      PlotSelectionChanged(this, null);
    }

    private void nodeTextBoxText_DrawText(object sender, DrawEventArgs e) {
      Node node = e.Node.Tag as Node;
      if (node != null) {
        Color color;
        if (node.IsVisible) {
          SensorNode sensorNode = node as SensorNode;
          if (plotMenuItem.Checked && sensorNode != null &&
            sensorPlotColors.TryGetValue(sensorNode.Sensor, out color)) {
            e.TextColor = color;
          }
        } else {
          e.TextColor = Color.DarkGray;
        }
      }
    }

    private void PlotSelectionChanged(object sender, EventArgs e) {
      List<ISensor> selected = new List<ISensor>();
      IDictionary<ISensor, Color> colors = new Dictionary<ISensor, Color>();
      int colorIndex = 0;
      foreach (TreeNodeAdv node in treeView.AllNodes) {
        SensorNode sensorNode = node.Tag as SensorNode;
        if (sensorNode != null) {
          if (sensorNode.Plot) {
            colors.Add(sensorNode.Sensor,
              plotColorPalette[colorIndex % plotColorPalette.Length]);
            selected.Add(sensorNode.Sensor);
          }
          colorIndex++;
        }
      }
      sensorPlotColors = colors;
      plotPanel.SetSensors(selected, colors);
    }

    private void nodeTextBoxText_EditorShowing(object sender,
      CancelEventArgs e) {
      e.Cancel = !(treeView.CurrentNode != null &&
        (treeView.CurrentNode.Tag is SensorNode ||
         treeView.CurrentNode.Tag is HardwareNode));
    }

    private void nodeCheckBox_IsVisibleValueNeeded(object sender,
      NodeControlValueEventArgs e) {
      SensorNode node = e.Node.Tag as SensorNode;
      e.Value = (node != null) && plotMenuItem.Checked;
    }

    private void exitClick(object sender, EventArgs e) {
      Close();
    }

    float MaxTemp(Computer computer, HardwareType type) {
      var gpus = computer.Hardware.Where(x => x.HardwareType == type).ToArray();
      if (gpus.Any()) {
        float t = 0;
        foreach (var gpu in gpus) {
          var temps = gpu.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
          if (temps.Any()) {
            var temp = temps.Max(x => x.Value.Value);
            t = Math.Max(temp, t);
          }

          foreach (var sh in gpu.SubHardware) {
            temps = sh.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
            if (temps.Any()) {
              var temp = temps.Max(x => x.Value.Value);
              t = Math.Max(temp, t);
            }
          }
        }

        return t;
      } else {
        return 0;
      }
    }
    float UsageInPercent(Computer computer, HardwareType type, string Name) {
      int n = 0;
      float p = -1;
      var elements = computer.Hardware.Where(device => device.HardwareType == type).ToArray();
      if (elements.Count() > 0) {
        foreach (var hardware in elements) {
          var temps = hardware.Sensors.Where(x => x.Name == Name
              && x.SensorType == SensorType.Load).ToArray();
          if (temps.Count() != 0) {
            n++;
            p = temps.Average(temp => temp.Value.Value);
          }
        }
      }
      return n > 0 ? p / n : 0;
    }
    float UsageInPercent(Computer computer, HardwareType type) {

      int n = 0;
      float p = -1;
      var elements = computer.Hardware.Where(x => x.HardwareType == type).ToArray();
      if (elements.Count() > 0) {


        foreach (var hardware in elements) {

          var temps = hardware.Sensors.Where(x => x.SensorType == SensorType.Load).ToArray();
          if (temps.Count() != 0) {
            n++;
            p = temps.Average(temp => temp.Value.Value);
          }

        }
      }
      return n > 0 ? p / n : 0;
    }
    float AvgTemp(Computer computer, HardwareType type) {
      var gpus = computer.Hardware.Where(x => x.HardwareType == type).ToArray();
      if (gpus.Any()) {
        int n = 0;
        float t = 0;
        foreach (var gpu in gpus) {
          var temps = gpu.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
          if (temps.Any()) {
            var temp = temps.Average(x => x.Value.Value);
            t += temp;
            n++;
          }

          foreach (var sh in gpu.SubHardware) {
            temps = sh.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
            if (temps.Any()) {
              var temp = temps.Average(x => x.Value.Value);
              t += temp;
              n++;
            }
          }
        }

        if (n > 0) {
          return t / (float)n;
        } else {
          return 0;
        }
      } else {
        return 0;
      }
    }

    private int delayCount = 0;

    public void timer_Tick(object sender, EventArgs e) {
      computer.Accept(updateVisitor);

      {
        /*string tmp = "";
        tmp += string.Format("{0};", (int)MaxTemp(computer, HardwareType.GpuAti) + AvgTemp(computer, HardwareType.GpuNvidia));
        tmp += string.Format("{0};", (int)MaxTemp(computer, HardwareType.CPU));
        tmp += string.Format("{0};", (int)MaxTemp(computer, HardwareType.Mainboard));
        tmp += string.Format("{0};", (int)MaxTemp(computer, HardwareType.HDD));
        tmp += string.Format("{0};", (int)(UsageInPercent(computer, HardwareType.GpuAti, "GPU Core") + UsageInPercent(computer, HardwareType.GpuNvidia, "GPU Core")));
        tmp += string.Format("{0};", (int)UsageInPercent(computer, HardwareType.CPU, "CPU Total"));
        tmp += string.Format("{0};", (int)UsageInPercent(computer, HardwareType.RAM, "Memory"));
        tmp += string.Format("{0};", (int)UsageInPercent(computer, HardwareType.GpuAti, "GPU Memory") + UsageInPercent(computer, HardwareType.GpuNvidia, "GPU Memory"));*/

        var gpuMaxTemp = Math.Max(
            (int)MaxTemp(computer, HardwareType.GpuNvidia),
            (int)MaxTemp(computer, HardwareType.GpuAti)
        );
        var gpuMaxUsage = Math.Max(
          (int)UsageInPercent(computer, HardwareType.GpuAti, "GPU Core"),
          (int)UsageInPercent(computer, HardwareType.GpuNvidia, "GPU Core")
        );
        var gpuMaxMemory = Math.Max(
          (int)UsageInPercent(computer, HardwareType.GpuAti, "GPU Memory"),
          (int)UsageInPercent(computer, HardwareType.GpuNvidia, "GPU Memory")
        );

        List <float> data = new List<float>
        {
            (int)MaxTemp(computer, HardwareType.CPU),
            gpuMaxTemp,
            (int)MaxTemp(computer, HardwareType.Mainboard),
            (int)MaxTemp(computer, HardwareType.HDD),
            (int)UsageInPercent(computer, HardwareType.CPU, "CPU Total"),
            (int)gpuMaxUsage,
            (int)UsageInPercent(computer, HardwareType.RAM, "Memory"),
            (int)gpuMaxMemory,

            // Right group.
            settings.GetValue("nMaxFan", 100),
            settings.GetValue("nMinFan", 20),
            settings.GetValue("nMaxTemp", 100),
            settings.GetValue("nMinTemp", 10),

            // Flags
            settings.GetValue("chkManualFan", false) ? 1 : 0,
            settings.GetValue("chkManualColor", false) ? 1 : 0,

            // Sliders.
            settings.GetValue("sldManualFan", 50),
            settings.GetValue("sldManualColor", 500),
            settings.GetValue("sldLedBrightness", 50),
            settings.GetValue("sldPlotInterval", 5),

            settings.GetValue("cboMaxTempSource", 0),
        };

        string tmp = string.Join(";", data.Select(T => T.ToString()).ToArray());

        serial.Write(Encoding.ASCII.GetBytes(tmp));
        serial.Write(Encoding.ASCII.GetBytes("E"));

        /*var gpus = computer.Hardware.Where(x => x.HardwareType == HardwareType.GpuAti || x.HardwareType == HardwareType.GpuNvidia).ToArray();
        if (gpus.Any())
        {
            var gpu = gpus.First();
            var temps = gpu.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
            if (temps.Any())
            {
                var temp = temps.Average(x => x.Value.Value);
                tmp += string.Format("gpu: {0}C\r", (int)temp);
            }
        }

        var cpus = computer.Hardware.Where(x => x.HardwareType == HardwareType.CPU).ToArray();

        if (cpus.Any())
        {
            var cpu = cpus.First();
            var temps = cpu.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
            if (temps.Any())
            {
                var temp = temps.Average(x => x.Value.Value);
                tmp += string.Format("cpu: {0}C\r", (int)temp);
            }
        }

        var mainboard = computer.Hardware.Where(x => x.HardwareType == HardwareType.Mainboard).Single();
        {
            var temps = mainboard.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
            if (temps.Any())
            {
                var temp = temps.Average(x => x.Value.Value);
                tmp += string.Format("mainboard: {0}C\r", (int)temp);
            }
        }

        var hdds = computer.Hardware.Where(x => x.HardwareType == HardwareType.CPU).ToArray();

        if (hdds.Any())
        {
            var cpu = cpus.First();
            var temps = cpu.Sensors.Where(x => x.SensorType == SensorType.Temperature).ToArray();
            if (temps.Any())
            {
                var temp = temps.Average(x => x.Value.Value);
                tmp += string.Format("hdd: {0}C\r", (int)temp);
            }
        }*/
      }

      treeView.Invalidate();
      plotPanel.InvalidatePlot();
      systemTray.Redraw();
      if (gadget != null) {
        gadget.Redraw();
      }

      if (wmiProvider != null) {
        wmiProvider.Update();
      }

      if (logSensors.Value && delayCount >= 4) {
        logger.Log();
      }

      if (delayCount < 4) {
        delayCount++;
      }
    }

    public void SaveConfiguration() {
      plotPanel.SetCurrentSettings();
      foreach (TreeColumn column in treeView.Columns) {
        settings.SetValue("treeView.Columns." + column.Header + ".Width",
  column.Width);
      }

      this.settings.SetValue("listenerPort", server.ListenerPort);

      string fileName = Path.ChangeExtension(
          System.Windows.Forms.Application.ExecutablePath, ".config");
      try {
        settings.Save(fileName);
      } catch (UnauthorizedAccessException) {
        MessageBox.Show("Access to the path '" + fileName + "' is denied. " +
          "The current settings could not be saved.",
          "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
      } catch (IOException) {
        MessageBox.Show("The path '" + fileName + "' is not writeable. " +
          "The current settings could not be saved.",
          "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
      }
    }

    private void MainForm_Load(object sender, EventArgs e) {
      Rectangle newBounds = new Rectangle {
        X = settings.GetValue("mainForm.Location.X", Location.X),
        Y = settings.GetValue("mainForm.Location.Y", Location.Y),
        Width = settings.GetValue("mainForm.Width", 470),
        Height = settings.GetValue("mainForm.Height", 640)
      };

      Rectangle fullWorkingArea = new Rectangle(int.MaxValue, int.MaxValue,
        int.MinValue, int.MinValue);

      foreach (Screen screen in Screen.AllScreens) {
        fullWorkingArea = Rectangle.Union(fullWorkingArea, screen.Bounds);
      }

      Rectangle intersection = Rectangle.Intersect(fullWorkingArea, newBounds);
      if (intersection.Width < 20 || intersection.Height < 20 ||
        !settings.Contains("mainForm.Location.X")
      ) {
        newBounds.X = (Screen.PrimaryScreen.WorkingArea.Width / 2) -
                      (newBounds.Width / 2);

        newBounds.Y = (Screen.PrimaryScreen.WorkingArea.Height / 2) -
                      (newBounds.Height / 2);
      }

      this.Bounds = newBounds;
    }

    private void MainForm_FormClosed(object sender, FormClosedEventArgs e) {
      Visible = false;
      systemTray.IsMainIconEnabled = false;
      timer.Enabled = false;
      computer.Close();
      SaveConfiguration();
      if (runWebServer.Value) {
        server.Quit();
      }

      systemTray.Dispose();
    }

    private void aboutMenuItem_Click(object sender, EventArgs e) {
      new AboutBox().ShowDialog();
    }

    private void treeView_Click(object sender, EventArgs e) {

      MouseEventArgs m = e as MouseEventArgs;
      if (m == null || m.Button != MouseButtons.Right) {
        return;
      }

      NodeControlInfo info = treeView.GetNodeControlInfoAt(
  new Point(m.X, m.Y)
);
      treeView.SelectedNode = info.Node;
      if (info.Node != null) {
        SensorNode node = info.Node.Tag as SensorNode;
        if (node != null && node.Sensor != null) {
          treeContextMenu.MenuItems.Clear();
          if (node.Sensor.Parameters.Length > 0) {
            MenuItem item = new MenuItem("Parameters...");
            item.Click += delegate (object obj, EventArgs args) {
              ShowParameterForm(node.Sensor);
            };
            treeContextMenu.MenuItems.Add(item);
          }
          if (nodeTextBoxText.EditEnabled) {
            MenuItem item = new MenuItem("Rename");
            item.Click += delegate (object obj, EventArgs args) {
              nodeTextBoxText.BeginEdit();
            };
            treeContextMenu.MenuItems.Add(item);
          }
          if (node.IsVisible) {
            MenuItem item = new MenuItem("Hide");
            item.Click += delegate (object obj, EventArgs args) {
              node.IsVisible = false;
            };
            treeContextMenu.MenuItems.Add(item);
          } else {
            MenuItem item = new MenuItem("Unhide");
            item.Click += delegate (object obj, EventArgs args) {
              node.IsVisible = true;
            };
            treeContextMenu.MenuItems.Add(item);
          }
          treeContextMenu.MenuItems.Add(new MenuItem("-"));
          {
            MenuItem item = new MenuItem("Show in Tray") {
              Checked = systemTray.Contains(node.Sensor)
            };
            item.Click += delegate (object obj, EventArgs args) {
              if (item.Checked) {
                systemTray.Remove(node.Sensor);
              } else {
                systemTray.Add(node.Sensor, true);
              }
            };
            treeContextMenu.MenuItems.Add(item);
          }
          if (gadget != null) {
            MenuItem item = new MenuItem("Show in Gadget") {
              Checked = gadget.Contains(node.Sensor)
            };
            item.Click += delegate (object obj, EventArgs args) {
              if (item.Checked) {
                gadget.Remove(node.Sensor);
              } else {
                gadget.Add(node.Sensor);
              }
            };
            treeContextMenu.MenuItems.Add(item);
          }
          if (node.Sensor.Control != null) {
            treeContextMenu.MenuItems.Add(new MenuItem("-"));
            IControl control = node.Sensor.Control;
            MenuItem controlItem = new MenuItem("Control");
            MenuItem defaultItem = new MenuItem("Default") {
              Checked = control.ControlMode == ControlMode.Default
            };
            controlItem.MenuItems.Add(defaultItem);
            defaultItem.Click += delegate (object obj, EventArgs args) {
              control.SetDefault();
            };
            MenuItem manualItem = new MenuItem("Manual");
            controlItem.MenuItems.Add(manualItem);
            manualItem.Checked = control.ControlMode == ControlMode.Software;
            for (int i = 0; i <= 100; i += 5) {
              if (i <= control.MaxSoftwareValue &&
                  i >= control.MinSoftwareValue) {
                MenuItem item = new MenuItem(i + " %") {
                  RadioCheck = true
                };
                manualItem.MenuItems.Add(item);
                item.Checked = control.ControlMode == ControlMode.Software &&
                  Math.Round(control.SoftwareValue) == i;
                int softwareValue = i;
                item.Click += delegate (object obj, EventArgs args) {
                  control.SetSoftware(softwareValue);
                };
              }
            }
            treeContextMenu.MenuItems.Add(controlItem);
          }

          treeContextMenu.Show(treeView, new Point(m.X, m.Y));
        }

        HardwareNode hardwareNode = info.Node.Tag as HardwareNode;
        if (hardwareNode != null && hardwareNode.Hardware != null) {
          treeContextMenu.MenuItems.Clear();

          if (nodeTextBoxText.EditEnabled) {
            MenuItem item = new MenuItem("Rename");
            item.Click += delegate (object obj, EventArgs args) {
              nodeTextBoxText.BeginEdit();
            };
            treeContextMenu.MenuItems.Add(item);
          }

          treeContextMenu.Show(treeView, new Point(m.X, m.Y));
        }
      }
    }

    private void saveReportMenuItem_Click(object sender, EventArgs e) {
      string report = computer.GetReport();
      if (saveFileDialog.ShowDialog() == DialogResult.OK) {
        using (TextWriter w = new StreamWriter(saveFileDialog.FileName)) {
          w.Write(report);
        }
      }
    }

    private void SysTrayHideShow() {
      Visible = !Visible;
      if (Visible) {
        Activate();
      }
    }

    protected override void WndProc(ref Message m) {
      const int WM_SYSCOMMAND = 0x112;
      const int SC_MINIMIZE = 0xF020;
      const int SC_CLOSE = 0xF060;

      if (minimizeToTray.Value &&
        m.Msg == WM_SYSCOMMAND && m.WParam.ToInt64() == SC_MINIMIZE) {
        SysTrayHideShow();
      } else if (minimizeOnClose.Value &&
        m.Msg == WM_SYSCOMMAND && m.WParam.ToInt64() == SC_CLOSE) {
        /*
         * Apparently the user wants to minimize rather than close
         * Now we still need to check if we're going to the tray or not
         * 
         * Note: the correct way to do this would be to send out SC_MINIMIZE,
         * but since the code here is so simple,
         * that would just be a waste of time.
         */
        if (minimizeToTray.Value) {
          SysTrayHideShow();
        } else {
          WindowState = FormWindowState.Minimized;
        }
      } else {
        base.WndProc(ref m);
      }
    }

    private void hideShowClick(object sender, EventArgs e) {
      SysTrayHideShow();
    }

    private void ShowParameterForm(ISensor sensor) {
      ParameterForm form = new ParameterForm {
        Parameters = sensor.Parameters
      };
      form.captionLabel.Text = sensor.Name;
      form.ShowDialog();
    }

    private void treeView_NodeMouseDoubleClick(object sender,
      TreeNodeAdvMouseEventArgs e) {
      SensorNode node = e.Node.Tag as SensorNode;
      if (node != null && node.Sensor != null &&
        node.Sensor.Parameters.Length > 0) {
        ShowParameterForm(node.Sensor);
      }
    }

    private void celsiusMenuItem_Click(object sender, EventArgs e) {
      celsiusMenuItem.Checked = true;
      fahrenheitMenuItem.Checked = false;
      unitManager.TemperatureUnit = TemperatureUnit.Celsius;
    }

    private void fahrenheitMenuItem_Click(object sender, EventArgs e) {
      celsiusMenuItem.Checked = false;
      fahrenheitMenuItem.Checked = true;
      unitManager.TemperatureUnit = TemperatureUnit.Fahrenheit;
    }

    private void sumbitReportMenuItem_Click(object sender, EventArgs e) {
      ReportForm form = new ReportForm {
        Report = computer.GetReport()
      };
      form.ShowDialog();
    }

    private void resetMinMaxMenuItem_Click(object sender, EventArgs e) {
      computer.Accept(new SensorVisitor(delegate (ISensor sensor) {
        sensor.ResetMin();
        sensor.ResetMax();
      }));
    }

    private void MainForm_MoveOrResize(object sender, EventArgs e) {
      if (WindowState != FormWindowState.Minimized) {
        settings.SetValue("mainForm.Location.X", Bounds.X);
        settings.SetValue("mainForm.Location.Y", Bounds.Y);
        settings.SetValue("mainForm.Width", Bounds.Width);
        settings.SetValue("mainForm.Height", Bounds.Height);
      }
    }

    private void resetClick(object sender, EventArgs e) {
      // disable the fallback MainIcon during reset, otherwise icon visibility
      // might be lost 
      systemTray.IsMainIconEnabled = false;
      computer.Close();
      computer.Open();
      // restore the MainIcon setting
      systemTray.IsMainIconEnabled = minimizeToTray.Value;
    }

    private void treeView_MouseMove(object sender, MouseEventArgs e) {
      selectionDragging = selectionDragging &
        (e.Button & (MouseButtons.Left | MouseButtons.Right)) > 0;

      if (selectionDragging) {
        treeView.SelectedNode = treeView.GetNodeAt(e.Location);
      }
    }

    private void treeView_MouseDown(object sender, MouseEventArgs e) {
      selectionDragging = true;
    }

    private void treeView_MouseUp(object sender, MouseEventArgs e) {
      selectionDragging = false;
    }

    private void serverPortMenuItem_Click(object sender, EventArgs e) {
      new PortForm(this).ShowDialog();
    }

    public HttpServer Server
    {
      get { return server; }
    }
    public Serial Serial
    {
      get { return serial; }
    }
    private void serialConfigMenuItem_Click(object sender, EventArgs e) {
      new SerialForm(this).ShowDialog();
    }

    private void runSerialMenuItem_Click(object sender, EventArgs e) {

    }

  }
}
