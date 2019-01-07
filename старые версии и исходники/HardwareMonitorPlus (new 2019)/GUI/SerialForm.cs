using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Text;
using System.Windows.Forms;

namespace OpenHardwareMonitor.GUI
{
    public partial class SerialForm : Form
    {
        private readonly MainForm _parent;
        private bool _loaded = false;

        public SerialForm(MainForm parent)
        {
            InitializeComponent();

            _parent = parent;
        }

        public string Port => (string)cboComPort.SelectedItem;

        private void okButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void SerialForm_Load(object sender, EventArgs e)
        {
            cboMaxTempSource.SelectedIndex = _parent.settings.GetValue(nameof(cboMaxTempSource), 0);

            LoadComPortNames();

            // Right group.
            nMaxFan.Value = _parent.settings.GetValue(nameof(nMaxFan), 100);
            nMinFan.Value = _parent.settings.GetValue(nameof(nMinFan), 20);
            nMaxTemp.Value = _parent.settings.GetValue(nameof(nMaxTemp), 80);
            nMinTemp.Value = _parent.settings.GetValue(nameof(nMinTemp), 20);

            // Flags
            chkManualFan.Checked = _parent.settings.GetValue(nameof(chkManualFan), false);
            chkManualColor.Checked = _parent.settings.GetValue(nameof(chkManualColor), false);

            // Sliders.
            sldManualFan.Value = _parent.settings.GetValue(nameof(sldManualFan), 50);
            sldManualColor.Value = _parent.settings.GetValue(nameof(sldManualColor), 500);
            sldLedBrightness.Value = _parent.settings.GetValue(nameof(sldLedBrightness), 50);
            sldPlotInterval.Value = _parent.settings.GetValue(nameof(sldPlotInterval), 5);

            _loaded = true;
        }

        private void LoadComPortNames()
        {
            cboComPort.Items.AddRange(SerialPort.GetPortNames());

            if (cboComPort.Items.Count > 0)
            {
                if (cboComPort.Items.Contains(_parent.Serial.Port))
                {
                    cboComPort.SelectedItem = _parent.Serial.Port;
                }
                else
                {
                    if (!string.IsNullOrEmpty(_parent.Serial.Port))
                    {
                        cboComPort.Items.Add(_parent.Serial.Port);
                    }
                    else
                    {
                        cboComPort.SelectedIndex = 0;
                        _parent.Serial.Port = (string)cboComPort.SelectedItem;
                    }
                }
            }
        }

        private void SaveAndSend()
        {
            if (!_loaded)
            {
                return;
            }

            _parent.settings.SetValue(nameof(cboMaxTempSource), cboMaxTempSource.SelectedIndex);
            _parent.Serial.Port = (string)cboComPort.SelectedItem;

            // Right group.
            _parent.settings.SetValue(nameof(nMaxFan), (int)nMaxFan.Value);
            _parent.settings.SetValue(nameof(nMinFan), (int)nMinFan.Value);
            _parent.settings.SetValue(nameof(nMaxTemp), (int)nMaxTemp.Value);
            _parent.settings.SetValue(nameof(nMinTemp), (int)nMinTemp.Value);

            // Flags
            _parent.settings.SetValue(nameof(chkManualFan), chkManualFan.Checked);
            _parent.settings.SetValue(nameof(chkManualColor), chkManualColor.Checked);

            // Sliders.
            _parent.settings.SetValue(nameof(sldManualFan), sldManualFan.Value);
            _parent.settings.SetValue(nameof(sldManualColor), sldManualColor.Value);
            _parent.settings.SetValue(nameof(sldLedBrightness), sldLedBrightness.Value);
            _parent.settings.SetValue(nameof(sldPlotInterval), sldPlotInterval.Value);
            _parent.SaveConfiguration();

            // Push data.
            _parent.SendData(false);
        }

        #region Event Handlers

        private void sldManualFan_ValueChanged(object sender, EventArgs e)
        {
            lblManualFanValue.Text = sldManualFan.Value.ToString();
            SaveAndSend();
        }
        private void sldManualColor_ValueChanged(object sender, EventArgs e)
        {
            lblManualColorValue.Text = sldManualColor.Value.ToString();
            SaveAndSend();
        }
        private void sldLedBrightness_ValueChanged(object sender, EventArgs e)
        {
            lblLedBrightnessValue.Text = sldLedBrightness.Value.ToString();
            SaveAndSend();
        }
        private void sldPlotInterval_ValueChanged(object sender, EventArgs e)
        {
            lblPlotIntervalValue.Text = sldPlotInterval.Value.ToString();
            SaveAndSend();
        }
        private void nMaxFan_ValueChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void nMinFan_ValueChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void nMinTemp_ValueChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void nMaxTemp_ValueChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void chkManualFan_CheckedChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void chkManualColor_CheckedChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void portBox_TextChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void cboComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }
        private void cboMaxTempSource_SelectedIndexChanged(object sender, EventArgs e)
        {
            SaveAndSend();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }
    }

    #endregion

}
