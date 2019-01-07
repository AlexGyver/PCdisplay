namespace OpenHardwareMonitor.GUI
{
    partial class SerialForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SerialForm));
            this.label1 = new System.Windows.Forms.Label();
            this.okButton = new System.Windows.Forms.Button();
            this.chkManualFan = new System.Windows.Forms.CheckBox();
            this.sldManualFan = new System.Windows.Forms.TrackBar();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.sldManualColor = new System.Windows.Forms.TrackBar();
            this.chkManualColor = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.sldLedBrightness = new System.Windows.Forms.TrackBar();
            this.label13 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.sldPlotInterval = new System.Windows.Forms.TrackBar();
            this.label15 = new System.Windows.Forms.Label();
            this.nMaxFan = new System.Windows.Forms.NumericUpDown();
            this.nMinFan = new System.Windows.Forms.NumericUpDown();
            this.nMinTemp = new System.Windows.Forms.NumericUpDown();
            this.nMaxTemp = new System.Windows.Forms.NumericUpDown();
            this.lblManualFanValue = new System.Windows.Forms.Label();
            this.lblManualColorValue = new System.Windows.Forms.Label();
            this.lblLedBrightnessValue = new System.Windows.Forms.Label();
            this.lblPlotIntervalValue = new System.Windows.Forms.Label();
            this.cboComPort = new System.Windows.Forms.ComboBox();
            this.cboMaxTempSource = new System.Windows.Forms.ComboBox();
            this.label16 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.sldManualFan)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldManualColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldLedBrightness)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldPlotInterval)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMaxFan)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMinFan)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMinTemp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMaxTemp)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "PORT address";
            // 
            // okButton
            // 
            this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.okButton.Location = new System.Drawing.Point(575, 391);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "Close";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // chkManualFan
            // 
            this.chkManualFan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chkManualFan.AutoSize = true;
            this.chkManualFan.Location = new System.Drawing.Point(12, 79);
            this.chkManualFan.Name = "chkManualFan";
            this.chkManualFan.Size = new System.Drawing.Size(85, 17);
            this.chkManualFan.TabIndex = 3;
            this.chkManualFan.Text = "Manual FAN";
            this.chkManualFan.UseVisualStyleBackColor = true;
            this.chkManualFan.CheckedChanged += new System.EventHandler(this.chkManualFan_CheckedChanged);
            // 
            // sldManualFan
            // 
            this.sldManualFan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.sldManualFan.Location = new System.Drawing.Point(12, 115);
            this.sldManualFan.Maximum = 100;
            this.sldManualFan.Name = "sldManualFan";
            this.sldManualFan.Size = new System.Drawing.Size(294, 45);
            this.sldManualFan.TabIndex = 4;
            this.sldManualFan.TickFrequency = 5;
            this.sldManualFan.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.sldManualFan.ValueChanged += new System.EventHandler(this.sldManualFan_ValueChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(391, 15);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(256, 256);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 5;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // sldManualColor
            // 
            this.sldManualColor.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.sldManualColor.Location = new System.Drawing.Point(12, 202);
            this.sldManualColor.Maximum = 1000;
            this.sldManualColor.Name = "sldManualColor";
            this.sldManualColor.Size = new System.Drawing.Size(294, 45);
            this.sldManualColor.TabIndex = 7;
            this.sldManualColor.TickFrequency = 25;
            this.sldManualColor.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.sldManualColor.ValueChanged += new System.EventHandler(this.sldManualColor_ValueChanged);
            // 
            // chkManualColor
            // 
            this.chkManualColor.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.chkManualColor.AutoSize = true;
            this.chkManualColor.Location = new System.Drawing.Point(12, 166);
            this.chkManualColor.Name = "chkManualColor";
            this.chkManualColor.Size = new System.Drawing.Size(101, 17);
            this.chkManualColor.TabIndex = 6;
            this.chkManualColor.Text = "Manual COLOR";
            this.chkManualColor.UseVisualStyleBackColor = true;
            this.chkManualColor.CheckedChanged += new System.EventHandler(this.chkManualColor_CheckedChanged);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(342, 219);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "FAN min";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(339, 54);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(50, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "FAN max";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(388, 276);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(56, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "TEMP min";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(534, 276);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 13);
            this.label5.TabIndex = 15;
            this.label5.Text = "TEMP max";
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(21, 99);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(13, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "0";
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(281, 99);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(25, 13);
            this.label7.TabIndex = 17;
            this.label7.Text = "100";
            // 
            // label8
            // 
            this.label8.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(275, 186);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(31, 13);
            this.label8.TabIndex = 19;
            this.label8.Text = "1000";
            // 
            // label9
            // 
            this.label9.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(21, 186);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(13, 13);
            this.label9.TabIndex = 18;
            this.label9.Text = "0";
            // 
            // label10
            // 
            this.label10.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(9, 250);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(79, 13);
            this.label10.TabIndex = 20;
            this.label10.Text = "LED brightness";
            // 
            // label11
            // 
            this.label11.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(281, 272);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(25, 13);
            this.label11.TabIndex = 23;
            this.label11.Text = "100";
            // 
            // label12
            // 
            this.label12.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(21, 272);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(13, 13);
            this.label12.TabIndex = 22;
            this.label12.Text = "0";
            // 
            // sldLedBrightness
            // 
            this.sldLedBrightness.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.sldLedBrightness.Location = new System.Drawing.Point(12, 288);
            this.sldLedBrightness.Maximum = 100;
            this.sldLedBrightness.Name = "sldLedBrightness";
            this.sldLedBrightness.Size = new System.Drawing.Size(294, 45);
            this.sldLedBrightness.TabIndex = 21;
            this.sldLedBrightness.TickFrequency = 5;
            this.sldLedBrightness.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.sldLedBrightness.ValueChanged += new System.EventHandler(this.sldLedBrightness_ValueChanged);
            // 
            // label13
            // 
            this.label13.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(287, 357);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(19, 13);
            this.label13.TabIndex = 27;
            this.label13.Text = "10";
            // 
            // label14
            // 
            this.label14.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(21, 357);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(13, 13);
            this.label14.TabIndex = 26;
            this.label14.Text = "0";
            // 
            // sldPlotInterval
            // 
            this.sldPlotInterval.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.sldPlotInterval.Location = new System.Drawing.Point(12, 373);
            this.sldPlotInterval.Name = "sldPlotInterval";
            this.sldPlotInterval.Size = new System.Drawing.Size(294, 45);
            this.sldPlotInterval.TabIndex = 25;
            this.sldPlotInterval.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.sldPlotInterval.ValueChanged += new System.EventHandler(this.sldPlotInterval_ValueChanged);
            // 
            // label15
            // 
            this.label15.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(9, 335);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(81, 13);
            this.label15.TabIndex = 24;
            this.label15.Text = "CHART interval";
            // 
            // nMaxFan
            // 
            this.nMaxFan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nMaxFan.Location = new System.Drawing.Point(341, 31);
            this.nMaxFan.Name = "nMaxFan";
            this.nMaxFan.Size = new System.Drawing.Size(48, 20);
            this.nMaxFan.TabIndex = 28;
            this.nMaxFan.ValueChanged += new System.EventHandler(this.nMaxFan_ValueChanged);
            // 
            // nMinFan
            // 
            this.nMinFan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nMinFan.Location = new System.Drawing.Point(341, 235);
            this.nMinFan.Name = "nMinFan";
            this.nMinFan.Size = new System.Drawing.Size(48, 20);
            this.nMinFan.TabIndex = 29;
            this.nMinFan.ValueChanged += new System.EventHandler(this.nMinFan_ValueChanged);
            // 
            // nMinTemp
            // 
            this.nMinTemp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nMinTemp.Location = new System.Drawing.Point(450, 274);
            this.nMinTemp.Name = "nMinTemp";
            this.nMinTemp.Size = new System.Drawing.Size(48, 20);
            this.nMinTemp.TabIndex = 30;
            this.nMinTemp.ValueChanged += new System.EventHandler(this.nMinTemp_ValueChanged);
            // 
            // nMaxTemp
            // 
            this.nMaxTemp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.nMaxTemp.Location = new System.Drawing.Point(599, 274);
            this.nMaxTemp.Name = "nMaxTemp";
            this.nMaxTemp.Size = new System.Drawing.Size(48, 20);
            this.nMaxTemp.TabIndex = 31;
            this.nMaxTemp.ValueChanged += new System.EventHandler(this.nMaxTemp_ValueChanged);
            // 
            // lblManualFanValue
            // 
            this.lblManualFanValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblManualFanValue.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblManualFanValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lblManualFanValue.Location = new System.Drawing.Point(139, 144);
            this.lblManualFanValue.Name = "lblManualFanValue";
            this.lblManualFanValue.Size = new System.Drawing.Size(41, 16);
            this.lblManualFanValue.TabIndex = 32;
            this.lblManualFanValue.Text = "100";
            this.lblManualFanValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblManualColorValue
            // 
            this.lblManualColorValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblManualColorValue.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblManualColorValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lblManualColorValue.Location = new System.Drawing.Point(139, 231);
            this.lblManualColorValue.Name = "lblManualColorValue";
            this.lblManualColorValue.Size = new System.Drawing.Size(41, 16);
            this.lblManualColorValue.TabIndex = 33;
            this.lblManualColorValue.Text = "100";
            this.lblManualColorValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblLedBrightnessValue
            // 
            this.lblLedBrightnessValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblLedBrightnessValue.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblLedBrightnessValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lblLedBrightnessValue.Location = new System.Drawing.Point(139, 317);
            this.lblLedBrightnessValue.Name = "lblLedBrightnessValue";
            this.lblLedBrightnessValue.Size = new System.Drawing.Size(41, 16);
            this.lblLedBrightnessValue.TabIndex = 34;
            this.lblLedBrightnessValue.Text = "100";
            this.lblLedBrightnessValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblPlotIntervalValue
            // 
            this.lblPlotIntervalValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.lblPlotIntervalValue.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.lblPlotIntervalValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lblPlotIntervalValue.Location = new System.Drawing.Point(139, 402);
            this.lblPlotIntervalValue.Name = "lblPlotIntervalValue";
            this.lblPlotIntervalValue.Size = new System.Drawing.Size(41, 16);
            this.lblPlotIntervalValue.TabIndex = 35;
            this.lblPlotIntervalValue.Text = "100";
            this.lblPlotIntervalValue.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // cboComPort
            // 
            this.cboComPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboComPort.FormattingEnabled = true;
            this.cboComPort.Location = new System.Drawing.Point(106, 12);
            this.cboComPort.Name = "cboComPort";
            this.cboComPort.Size = new System.Drawing.Size(95, 21);
            this.cboComPort.TabIndex = 36;
            this.cboComPort.SelectedIndexChanged += new System.EventHandler(this.cboComPort_SelectedIndexChanged);
            // 
            // cboMaxTempSource
            // 
            this.cboMaxTempSource.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboMaxTempSource.FormattingEnabled = true;
            this.cboMaxTempSource.Items.AddRange(new object[] {
            "CPU only",
            "GPU only",
            "Max (CPU, GPU)",
            "Temp 1",
            "Temp 2"});
            this.cboMaxTempSource.Location = new System.Drawing.Point(106, 39);
            this.cboMaxTempSource.Name = "cboMaxTempSource";
            this.cboMaxTempSource.Size = new System.Drawing.Size(95, 21);
            this.cboMaxTempSource.TabIndex = 37;
            this.cboMaxTempSource.SelectedIndexChanged += new System.EventHandler(this.cboMaxTempSource_SelectedIndexChanged);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(12, 42);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(72, 13);
            this.label16.TabIndex = 38;
            this.label16.Text = "TEMP source";
            // 
            // SerialForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.okButton;
            this.ClientSize = new System.Drawing.Size(662, 426);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.cboMaxTempSource);
            this.Controls.Add(this.cboComPort);
            this.Controls.Add(this.lblPlotIntervalValue);
            this.Controls.Add(this.lblLedBrightnessValue);
            this.Controls.Add(this.lblManualColorValue);
            this.Controls.Add(this.lblManualFanValue);
            this.Controls.Add(this.nMaxTemp);
            this.Controls.Add(this.nMinTemp);
            this.Controls.Add(this.nMinFan);
            this.Controls.Add(this.nMaxFan);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.sldPlotInterval);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.sldLedBrightness);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.sldManualColor);
            this.Controls.Add(this.chkManualColor);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.sldManualFan);
            this.Controls.Add(this.chkManualFan);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "SerialForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Serial Configuration";
            this.Load += new System.EventHandler(this.SerialForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.sldManualFan)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldManualColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldLedBrightness)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.sldPlotInterval)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMaxFan)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMinFan)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMinTemp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nMaxTemp)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.CheckBox chkManualFan;
        private System.Windows.Forms.TrackBar sldManualFan;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TrackBar sldManualColor;
        private System.Windows.Forms.CheckBox chkManualColor;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TrackBar sldLedBrightness;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TrackBar sldPlotInterval;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.NumericUpDown nMaxFan;
        private System.Windows.Forms.NumericUpDown nMinFan;
        private System.Windows.Forms.NumericUpDown nMinTemp;
        private System.Windows.Forms.NumericUpDown nMaxTemp;
        private System.Windows.Forms.Label lblManualFanValue;
        private System.Windows.Forms.Label lblManualColorValue;
        private System.Windows.Forms.Label lblLedBrightnessValue;
        private System.Windows.Forms.Label lblPlotIntervalValue;
        private System.Windows.Forms.ComboBox cboComPort;
        private System.Windows.Forms.ComboBox cboMaxTempSource;
        private System.Windows.Forms.Label label16;
    }
}