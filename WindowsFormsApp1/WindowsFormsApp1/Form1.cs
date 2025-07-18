using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        private readonly List<TextBox> _myTextBoxes_Sign1;
        private readonly List<TextBox> _myTextBoxes_Sign2;
        private readonly List<TextBox> _myTextBoxes_Angle;
        public Form1()
        {
            InitializeComponent();

            _myTextBoxes_Angle = new List<TextBox>
            {
                textBox7,textBox10,textBox13,textBox16,textBox19,textBox22,textBox25,textBox28
            };
            _myTextBoxes_Sign1 = new List<TextBox>
            {
                textBox8,textBox11,textBox14,textBox17,textBox20,textBox23,textBox26,textBox29
            };
            _myTextBoxes_Sign2 = new List<TextBox>
            {
                textBox9,textBox12,textBox15,textBox18,textBox21,textBox24,textBox27,textBox30
            };
            
            
        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void flowLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tableLayoutPanel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void textBox7_TextChanged(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void textBox29_TextChanged(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void SignItem_SelectedIndexChanged(object sender, EventArgs e)
        {
            string value = SignItem.SelectedItem?.ToString();
            if (value == "PWM")
            {
                label11.Visible = true;
                Label11_textBox1.Visible = true;
                label12.Visible = false;
                Label12_textBox1.Visible = false;
            }
            else if(value == "PWM  电压"){
                label11.Visible = true;
                Label11_textBox1.Visible = true;
                label12.Visible = false;
                Label12_textBox1.Visible = false;
            }
            else if (value == "电压 PWM")
            {
                label11.Visible = false;
                Label11_textBox1.Visible = false;
                label12.Visible = true;
                Label12_textBox1.Visible = true;
            }
            else if(value == "PWM  PWM")
            {
                label11.Visible = true;
                Label11_textBox1.Visible = true;
                label12.Visible = true;
                Label12_textBox1.Visible = true;
            }
            else
            {
                label11.Visible = false;
                Label11_textBox1.Visible = false;
                label12.Visible = false;
                Label12_textBox1.Visible = false;
            }
                
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }
        /// <单双路选择>
        /// 
        /// </单双路选择>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MuchSign_SelectedIndexChanged(object sender, EventArgs e)
        {
            string value = MuchSignBox.SelectedItem?.ToString();
            string value2 = SignItem.SelectedItem?.ToString();
            if (value == "单路"){
                SignItem.Enabled = true;
                SignItem.Items.Clear();
                SignItem.Items.Add("电压");
                SignItem.Items.Add("PWM");
                foreach (var tb in _myTextBoxes_Angle)
                {
                    tb.Enabled = true;
                }
                foreach (var tb in _myTextBoxes_Sign1)
                {
                    tb.Enabled = true;
                }
                foreach (var tb in _myTextBoxes_Sign2)
                {
                    tb.Enabled = false;
                }
            }else if (value == "双路")
            {
                SignItem.Enabled = true;
                SignItem.Items.Clear();
                SignItem.Items.Add("电压 电压");
                SignItem.Items.Add("电压 PWM");
                SignItem.Items.Add("PWM  PWM");
                SignItem.Items.Add("PWM  电压");
                foreach (var tb in _myTextBoxes_Angle)
                {
                    tb.Enabled = true;
                }
                foreach (var tb in _myTextBoxes_Sign1)
                {
                    tb.Enabled = true;
                }
                foreach (var tb in _myTextBoxes_Sign2)
                {
                    tb.Enabled = true;
                }
            }

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void label12_Click(object sender, EventArgs e)
        {

        }

        private void textBox12_TextChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void shunniBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (shunniBox.SelectedItem?.ToString() == "顺时针")
            {
                frock1.Directionz = Direction.Sclockwise;
            }
            else if (shunniBox.SelectedItem?.ToString() == "逆时针")
            {
                frock1.Directionz = Direction.Counterclockwise;
            }
        }
        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void frock1_Click(object sender, EventArgs e)
        {

        }

        private void ZeroPointSetBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (shunniBox.SelectedItem?.ToString() == "顺时针")
            {
                switch (ZeroPointSetBox.SelectedItem?.ToString())
                {
                    case "上":
                        frock1.ZeroSet = ZeroSetting.Two70;
                        break;
                    case "下":
                        frock1.ZeroSet = ZeroSetting.Ninty;
                        break;
                    case "左":
                        frock1.ZeroSet = ZeroSetting.One80;
                        break;
                    case "右":
                        frock1.ZeroSet = ZeroSetting.Zero;
                        break;
                }
            }
            else if (shunniBox.SelectedItem?.ToString() == "逆时针") 
            {
                switch (ZeroPointSetBox.SelectedItem?.ToString())
                {
                    case "上":
                        frock1.ZeroSet = ZeroSetting.Ninty;
                        break;
                    case "下":
                        frock1.ZeroSet = ZeroSetting.Two70;
                        break;
                    case "左":
                        frock1.ZeroSet = ZeroSetting.One80;
                        break;
                    case "右":
                        frock1.ZeroSet = ZeroSetting.Zero;
                        break;
                }
            }
                
        }
    }
}
