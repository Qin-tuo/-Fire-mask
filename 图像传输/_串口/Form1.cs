using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Windows.Forms;
namespace _串口
{
    public class Form1 : Form
    {
        private long recCnt;
        private byte[] bytes;
        private int bufferlen;
        private bool Listening;
        private bool closing;
        private List<byte> bufferList = new List<byte>(40960);
        private byte[] _buffer = new byte[102400];
        private short frame;
        private IContainer components;
        private SerialPort SerialPort;
        private Button btnOpen;
        private ComboBox comList;
        private ComboBox baudList;
        private Timer Timer;
        private PictureBox picBox;
        private TextBox widthBox;
        private TextBox heightBox;
        private ComboBox rotBox;
        private Label label1;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label5;
        private GroupBox groupBox1;
        private GroupBox groupBox2;
        private ComboBox amplifyBox;
        private Label label6;
        private Label cntlabel;
        private Label framelabel;
        private Button button1;
        public Form1()
        {
            this.InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            try
            {
                this.picBox.Size = new Size(Convert.ToInt32(this.widthBox.Text) * Convert.ToInt32(this.amplifyBox.Text), Convert.ToInt32(this.heightBox.Text) * Convert.ToInt32(this.amplifyBox.Text));
                string[] portNames = SerialPort.GetPortNames();
                Array.Sort<string>(portNames);
                this.comList.Items.AddRange(portNames);
                this.comList.SelectedIndex = ((this.comList.Items.Count > 0) ? 0 : -1);
            }
            catch
            {
                MessageBox.Show("请检查串口连接！");
            }
        }
        public static Bitmap ToGrayBitmap(byte[] rawValues, int width, int height)
        {
            Bitmap bitmap = new Bitmap(width, height, PixelFormat.Format8bppIndexed);
            BitmapData bitmapData = bitmap.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
            int stride = bitmapData.Stride;
            int num = stride - width;
            IntPtr scan = bitmapData.Scan0;
            int num2 = stride * height;
            int num3 = 0;
            int num4 = 0;
            byte[] array = new byte[num2];
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    array[num3++] = rawValues[num4++];
                }
                num3 += num;
            }
            Marshal.Copy(array, 0, scan, num2);
            bitmap.UnlockBits(bitmapData);
            ColorPalette palette;
            using (Bitmap bitmap2 = new Bitmap(1, 1, PixelFormat.Format8bppIndexed))
            {
                palette = bitmap2.Palette;
            }
            for (int k = 0; k < 256; k++)
            {
                palette.Entries[k] = Color.FromArgb(k, k, k);
            }
            bitmap.Palette = palette;
            return bitmap;
        }
        public static Bitmap ResizeImage(Bitmap bmp, int newW, int newH)
        {
            Bitmap result;
            try
            {
                Bitmap bitmap = new Bitmap(newW, newH);
                Graphics graphics = Graphics.FromImage(bitmap);
                graphics.InterpolationMode = InterpolationMode.HighQualityBicubic;
                graphics.DrawImage(bmp, new Rectangle(0, 0, newW, newH), new Rectangle(0, 0, bmp.Width, bmp.Height), GraphicsUnit.Pixel);
                graphics.Dispose();
                result = bitmap;
            }
            catch
            {
                result = null;
            }
            return result;
        }
        public static Bitmap rotateImage(Bitmap b, float angle)
        {
            Bitmap bitmap = new Bitmap(b.Width, b.Height);
            Graphics graphics = Graphics.FromImage(bitmap);
            graphics.InterpolationMode = InterpolationMode.NearestNeighbor;
            graphics.TranslateTransform((float)b.Width / 2f, (float)b.Height / 2f);
            graphics.RotateTransform(angle);
            graphics.TranslateTransform(-(float)b.Width / 2f, -(float)b.Height / 2f);
            graphics.DrawImage(b, new Point(0, 0));
            return bitmap;
        }
        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (this.closing)
            {
                return;
            }
            try
            {
                this.Listening = true;
                base.Invoke(new WriteInvoke(Invo), new object[] { }); //参数传进数组
            }
            finally
            {
                this.Listening = false;
            }
        }

        delegate void WriteInvoke();
        private void Invo()
        {
            this.bufferlen = this.SerialPort.BytesToRead;
            this.bytes = new byte[this.bufferlen];
            this.SerialPort.Read(this.bytes, 0, this.bufferlen);
            this.recCnt += (long)this.bufferlen;
            this.cntlabel.Text = "R：" + Convert.ToString(this.recCnt) + "byte";
            this.bufferList.AddRange(this.bytes);
            while (this.bufferList.Count > 19202)
            {
                if (this.bufferList[0] == 170 && this.bufferList[1] == 3)
                {
                    this.bufferList.CopyTo(2, this._buffer, 0, 19200);
                    this.picBox.Refresh();
                    this.picBox.Image = Form1.ResizeImage(Form1.rotateImage(Form1.ToGrayBitmap(this._buffer, Convert.ToInt32(this.widthBox.Text), Convert.ToInt32(this.heightBox.Text)), (float)Convert.ToInt32(this.rotBox.Text)), Convert.ToInt32(this.widthBox.Text) * Convert.ToInt32(this.amplifyBox.Text), Convert.ToInt32(this.heightBox.Text) * Convert.ToInt32(this.amplifyBox.Text));
                    this.bufferList.RemoveRange(0, 19202);
                    this.frame += 1;
                }
                else
                {
                    this.bufferList.RemoveAt(0);
                }
            }
        }


        private void btnOpen_Click(object sender, EventArgs e)
        {
            if (!this.SerialPort.IsOpen)
            {
                try
                {
                    this.SerialPort.PortName = this.comList.Text;
                    this.SerialPort.BaudRate = Convert.ToInt32(this.baudList.Text);
                    this.SerialPort.DataBits = 8;
                    this.SerialPort.StopBits = StopBits.One;
                    this.SerialPort.Parity = Parity.None;
                    this.SerialPort.Open();
                    this.comList.Enabled = false;
                    this.baudList.Enabled = false;
                    this.btnOpen.Text = "断开";
                    this.Timer.Start();
                    return;
                }
                catch
                {
                    MessageBox.Show("打开串口失败！");
                    return;
                }
            }
            try
            {
                this.closing = true;
                while (this.Listening)
                {
                    Application.DoEvents();
                }
                this.SerialPort.Close();
                this.closing = false;
                this.comList.Enabled = true;
                this.baudList.Enabled = true;
                this.btnOpen.Text = "连接";
                this.recCnt = 0L;
                this.Timer.Stop();
            }
            catch
            {
                MessageBox.Show("关闭串口失败！");
            }
        }
        private void comList_MouseClick(object sender, MouseEventArgs e)
        {
            try
            {
                this.comList.Items.Clear();
                string[] portNames = SerialPort.GetPortNames();
                Array.Sort<string>(portNames);
                this.comList.Items.AddRange(portNames);
                this.comList.SelectedIndex = ((this.comList.Items.Count > 0) ? 0 : -1);
            }
            catch
            {
            }
        }
        private void Timer_Tick(object sender, EventArgs e)
        {
            this.picBox.Refresh();
            this.framelabel.Text = "frame: " + ((double)((float)this.frame) / 0.5).ToString("f0") + " fps";
            this.frame = 0;
        }
        private void amplifyBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.picBox.Size = new Size(Convert.ToInt32(this.widthBox.Text) * Convert.ToInt32(this.amplifyBox.Text), Convert.ToInt32(this.heightBox.Text) * Convert.ToInt32(this.amplifyBox.Text));
        }
        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("  本软件由C#编写。\r\n");
        }
        protected override void Dispose(bool disposing)
        {
            if (disposing && this.components != null)
            {
                this.components.Dispose();
            }
            base.Dispose(disposing);
        }
        private void InitializeComponent()
        {
            this.components = new Container();
            ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(Form1));
            this.SerialPort = new SerialPort(this.components);
            this.btnOpen = new Button();
            this.comList = new ComboBox();
            this.baudList = new ComboBox();
            this.Timer = new Timer(this.components);
            this.picBox = new PictureBox();
            this.widthBox = new TextBox();
            this.heightBox = new TextBox();
            this.rotBox = new ComboBox();
            this.label1 = new Label();
            this.label2 = new Label();
            this.label3 = new Label();
            this.label4 = new Label();
            this.label5 = new Label();
            this.groupBox1 = new GroupBox();
            this.groupBox2 = new GroupBox();
            this.label6 = new Label();
            this.amplifyBox = new ComboBox();
            this.cntlabel = new Label();
            this.framelabel = new Label();
            this.button1 = new Button();
            ((ISupportInitialize)this.picBox).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            base.SuspendLayout();
            this.SerialPort.DataReceived += new SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            this.btnOpen.Location = new Point(45, 113);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new Size(63, 23);
            this.btnOpen.TabIndex = 0;
            this.btnOpen.Text = "连接";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new EventHandler(this.btnOpen_Click);
            this.comList.ForeColor = SystemColors.MenuHighlight;
            this.comList.FormattingEnabled = true;
            this.comList.Location = new Point(67, 30);
            this.comList.Name = "comList";
            this.comList.Size = new Size(70, 20);
            this.comList.TabIndex = 1;
            this.comList.MouseClick += new MouseEventHandler(this.comList_MouseClick);
            this.baudList.ForeColor = SystemColors.MenuHighlight;
            this.baudList.FormattingEnabled = true;
            this.baudList.Items.AddRange(new object[]
            {
                "1200",
                "2400",
                "4800",
                "9600",
                "14400",
                "19200",
                "28800",
                "38400",
                "57600",
                "115200",
                "921600",
                //"1382400",
                "1500000"
            });
            this.baudList.Location = new Point(67, 69);
            this.baudList.Name = "baudList";
            this.baudList.Size = new Size(70, 20);
            this.baudList.TabIndex = 2;
            this.baudList.Text = "1500000";
            this.Timer.Interval = 500;
            this.Timer.Tick += new EventHandler(this.Timer_Tick);
            this.picBox.BackColor = SystemColors.ActiveCaptionText;
            this.picBox.Location = new Point(25, 19);
            this.picBox.Name = "picBox";
            this.picBox.Size = new Size(644, 474);
            this.picBox.TabIndex = 6;
            this.picBox.TabStop = false;
            this.widthBox.Location = new Point(55, 32);
            this.widthBox.Name = "widthBox";
            this.widthBox.Size = new Size(53, 21);
            this.widthBox.TabIndex = 7;
            this.widthBox.Text = "160";
            this.heightBox.Location = new Point(55, 69);
            this.heightBox.Name = "heightBox";
            this.heightBox.Size = new Size(53, 21);
            this.heightBox.TabIndex = 8;
            this.heightBox.Text = "120";
            this.rotBox.FormattingEnabled = true;
            this.rotBox.Items.AddRange(new object[]
            {
                "90",
                "-90",
                "180",
                "360"
            });
            this.rotBox.Location = new Point(55, 107);
            this.rotBox.Name = "rotBox";
            this.rotBox.Size = new Size(54, 20);
            this.rotBox.TabIndex = 9;
            this.rotBox.Text = "90";
            this.label1.AutoSize = true;
            this.label1.Location = new Point(32, 35);
            this.label1.Name = "label1";
            this.label1.Size = new Size(17, 12);
            this.label1.TabIndex = 10;
            this.label1.Text = "长";
            this.label2.AutoSize = true;
            this.label2.Location = new Point(32, 72);
            this.label2.Name = "label2";
            this.label2.Size = new Size(17, 12);
            this.label2.TabIndex = 10;
            this.label2.Text = "宽";
            this.label3.AutoSize = true;
            this.label3.Location = new Point(20, 110);
            this.label3.Name = "label3";
            this.label3.Size = new Size(29, 12);
            this.label3.TabIndex = 10;
            this.label3.Text = "旋转";
            this.label4.AutoSize = true;
            this.label4.Location = new Point(20, 72);
            this.label4.Name = "label4";
            this.label4.Size = new Size(41, 12);
            this.label4.TabIndex = 10;
            this.label4.Text = "波特率";
            this.label5.AutoSize = true;
            this.label5.Location = new Point(20, 33);
            this.label5.Name = "label5";
            this.label5.Size = new Size(29, 12);
            this.label5.TabIndex = 10;
            this.label5.Text = "端口";
            this.groupBox1.Controls.Add(this.heightBox);
            this.groupBox1.Controls.Add(this.widthBox);
            this.groupBox1.Controls.Add(this.amplifyBox);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.rotBox);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new Point(702, 45);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new Size(149, 187);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "图片参数";
            this.groupBox2.Controls.Add(this.baudList);
            this.groupBox2.Controls.Add(this.comList);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.btnOpen);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Location = new Point(702, 256);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new Size(149, 156);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "串口配置";
            this.label6.AutoSize = true;
            this.label6.Location = new Point(20, 148);
            this.label6.Name = "label6";
            this.label6.Size = new Size(29, 12);
            this.label6.TabIndex = 10;
            this.label6.Text = "放大";
            this.amplifyBox.FormattingEnabled = true;
            this.amplifyBox.Items.AddRange(new object[]
            {
                "1",
                "2",
                "3",
                "4"
            });
            this.amplifyBox.Location = new Point(55, 145);
            this.amplifyBox.Name = "amplifyBox";
            this.amplifyBox.Size = new Size(54, 20);
            this.amplifyBox.TabIndex = 9;
            this.amplifyBox.Text = "4";
            this.amplifyBox.SelectedIndexChanged += new EventHandler(this.amplifyBox_SelectedIndexChanged);
            this.cntlabel.AutoSize = true;
            this.cntlabel.Location = new Point(734, 429);
            this.cntlabel.Name = "cntlabel";
            this.cntlabel.Size = new Size(59, 12);
            this.cntlabel.TabIndex = 13;
            this.cntlabel.Text = "R: 0 byte";
            this.framelabel.AutoSize = true;
            this.framelabel.Location = new Point(733, 461);
            this.framelabel.Name = "framelabel";
            this.framelabel.Size = new Size(77, 12);
            this.framelabel.TabIndex = 14;
            this.framelabel.Text = "frame: 0 fps";
            this.button1.Location = new Point(724, 491);
            this.button1.Name = "button1";
            this.button1.Size = new Size(150, 19);
            this.button1.TabIndex = 15;
            this.button1.Text = "相关信息";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new EventHandler(this.button1_Click);
            base.AutoScaleDimensions = new SizeF(6f, 12f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(902, 522);
            base.Controls.Add(this.button1);
            base.Controls.Add(this.framelabel);
            base.Controls.Add(this.cntlabel);
            base.Controls.Add(this.groupBox2);
            base.Controls.Add(this.groupBox1);
            base.Controls.Add(this.picBox);
            this.DoubleBuffered = true;
            base.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
            base.Name = "Form1";
            this.Text = "STM串口传输图像";
            base.Load += new EventHandler(this.Form1_Load);
            ((ISupportInitialize)this.picBox).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            base.ResumeLayout(false);
            base.PerformLayout();
        }
    }
}
