namespace DiscreteCosineTransform
{
    partial class MainFrm
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
            this.components = new System.ComponentModel.Container();
            this.DCTMapImage = new System.Windows.Forms.PictureBox();
            this.CNMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.selectFullImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.toolStripStatusLabel5 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel6 = new System.Windows.Forms.ToolStripStatusLabel();
            this.label5 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.ImSelected = new System.Windows.Forms.PictureBox();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.ImageInput = new System.Windows.Forms.PictureBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.scalepercentage = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button5 = new System.Windows.Forms.Button();
            this.button6 = new System.Windows.Forms.Button();
            this.IDCTImage = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.DCTMapImage)).BeginInit();
            this.CNMenuStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ImSelected)).BeginInit();
            this.toolStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ImageInput)).BeginInit();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.IDCTImage)).BeginInit();
            this.SuspendLayout();
            // 
            // DCTMapImage
            // 
            this.DCTMapImage.ContextMenuStrip = this.CNMenuStrip;
            this.DCTMapImage.Location = new System.Drawing.Point(455, 352);
            this.DCTMapImage.Name = "DCTMapImage";
            this.DCTMapImage.Size = new System.Drawing.Size(256, 256);
            this.DCTMapImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.DCTMapImage.TabIndex = 38;
            this.DCTMapImage.TabStop = false;
            // 
            // CNMenuStrip
            // 
            this.CNMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.selectFullImageToolStripMenuItem,
            this.openFileToolStripMenuItem,
            this.closeToolStripMenuItem});
            this.CNMenuStrip.Name = "CNMenuStrip";
            this.CNMenuStrip.Size = new System.Drawing.Size(148, 70);
            // 
            // selectFullImageToolStripMenuItem
            // 
            this.selectFullImageToolStripMenuItem.Name = "selectFullImageToolStripMenuItem";
            this.selectFullImageToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.selectFullImageToolStripMenuItem.Text = "Select Image";
            this.selectFullImageToolStripMenuItem.Click += new System.EventHandler(this.selectFullImageToolStripMenuItem_Click);
            // 
            // openFileToolStripMenuItem
            // 
            this.openFileToolStripMenuItem.Name = "openFileToolStripMenuItem";
            this.openFileToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.openFileToolStripMenuItem.Text = "Preview";
            this.openFileToolStripMenuItem.Click += new System.EventHandler(this.openFileToolStripMenuItem_Click);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.closeToolStripMenuItem.Text = "Close";
            // 
            // toolStripStatusLabel5
            // 
            this.toolStripStatusLabel5.Name = "toolStripStatusLabel5";
            this.toolStripStatusLabel5.Size = new System.Drawing.Size(127, 17);
            this.toolStripStatusLabel5.Text = "Selected Part Dimensions";
            // 
            // toolStripStatusLabel6
            // 
            this.toolStripStatusLabel6.Name = "toolStripStatusLabel6";
            this.toolStripStatusLabel6.Size = new System.Drawing.Size(78, 17);
            this.toolStripStatusLabel6.Text = "Width X Height";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(452, 329);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 13);
            this.label5.TabIndex = 39;
            this.label5.Text = "DCT Map";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(452, 37);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 13);
            this.label2.TabIndex = 34;
            this.label2.Text = "Selected Image";
            // 
            // ImSelected
            // 
            this.ImSelected.ContextMenuStrip = this.CNMenuStrip;
            this.ImSelected.Location = new System.Drawing.Point(455, 53);
            this.ImSelected.Name = "ImSelected";
            this.ImSelected.Size = new System.Drawing.Size(256, 256);
            this.ImSelected.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.ImSelected.TabIndex = 32;
            this.ImSelected.TabStop = false;
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(78, 17);
            this.toolStripStatusLabel4.Text = "Width X Height";
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripLabel1});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(1001, 25);
            this.toolStrip1.TabIndex = 27;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(33, 22);
            this.toolStripLabel1.Text = "Open";
            this.toolStripLabel1.Click += new System.EventHandler(this.toolStripLabel1_Click);
            // 
            // ImageInput
            // 
            this.ImageInput.ContextMenuStrip = this.CNMenuStrip;
            this.ImageInput.Location = new System.Drawing.Point(11, 37);
            this.ImageInput.Name = "ImageInput";
            this.ImageInput.Size = new System.Drawing.Size(400, 600);
            this.ImageInput.TabIndex = 28;
            this.ImageInput.TabStop = false;
            this.ImageInput.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ImageInput_MouseMove);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabel3,
            this.toolStripStatusLabel4,
            this.toolStripStatusLabel5,
            this.toolStripStatusLabel6});
            this.statusStrip1.Location = new System.Drawing.Point(0, 674);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1001, 22);
            this.statusStrip1.TabIndex = 31;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(103, 17);
            this.toolStripStatusLabel1.Text = "Image Dimensions : ";
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(78, 17);
            this.toolStripStatusLabel2.Text = "Width X Height";
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(78, 17);
            this.toolStripStatusLabel3.Text = "After Scaling : ";
            // 
            // scalepercentage
            // 
            this.scalepercentage.Location = new System.Drawing.Point(135, 649);
            this.scalepercentage.Name = "scalepercentage";
            this.scalepercentage.Size = new System.Drawing.Size(26, 20);
            this.scalepercentage.TabIndex = 29;
            this.scalepercentage.Text = "25";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 652);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(100, 13);
            this.label1.TabIndex = 30;
            this.label1.Text = "Scaling Percentage";
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(736, 352);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(86, 33);
            this.button5.TabIndex = 44;
            this.button5.Text = "Fast DCT";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(828, 352);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(87, 34);
            this.button6.TabIndex = 45;
            this.button6.Text = "Fast IDCT";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // IDCTImage
            // 
            this.IDCTImage.ContextMenuStrip = this.CNMenuStrip;
            this.IDCTImage.Location = new System.Drawing.Point(736, 53);
            this.IDCTImage.Name = "IDCTImage";
            this.IDCTImage.Size = new System.Drawing.Size(256, 256);
            this.IDCTImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.IDCTImage.TabIndex = 46;
            this.IDCTImage.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(733, 37);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(99, 13);
            this.label3.TabIndex = 47;
            this.label3.Text = "Inverse DCT Image";
            // 
            // MainFrm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1001, 696);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.IDCTImage);
            this.Controls.Add(this.DCTMapImage);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ImSelected);
            this.Controls.Add(this.ImageInput);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.scalepercentage);
            this.Controls.Add(this.label1);
            this.Name = "MainFrm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Discrete Cosine Transform";
            this.Load += new System.EventHandler(this.MainFrm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.DCTMapImage)).EndInit();
            this.CNMenuStrip.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ImSelected)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ImageInput)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.IDCTImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox DCTMapImage;
        private System.Windows.Forms.ContextMenuStrip CNMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem selectFullImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel5;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox ImSelected;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
        private System.Windows.Forms.PictureBox ImageInput;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.TextBox scalepercentage;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.PictureBox IDCTImage;
        private System.Windows.Forms.Label label3;
    }
}

