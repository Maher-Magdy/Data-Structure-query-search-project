using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using System.Threading;

using System.Collections;
using System.Text.RegularExpressions;
namespace Data_Index_test0
{
    public partial class Form1 : Form
    {

        //public
        string location="" ;
        string treeLocation = "";
        string cPath="data index c++.exe";
        bool first = true;
        int noOfResultFiles = 0;
        int command = 0;
        void TextWrite(string name, string data)
        {
            string path = @name + ".txt";
            if (File.Exists(path))
            {
                File.Delete(path);
            }
            StreamWriter text = new StreamWriter(path);
            text.Write("{0}\r\n", data);
            text.Close();
        }
        public Form1()
        {
            InitializeComponent();
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if ((location == "")) { MessageBox.Show("Choose a file location first"); }

            else

            {
                TextWrite("The Path", location);//for inverse use newLocationString

                TextWrite("The Word", command.ToString());
                TextWrite("The Result", "not found");
                TextWrite("Done", "0");
                TextWrite("Read", "1");
               
               
                //call c++ only one time
               if (first == true) { Process.Start(cPath); first = false; }

                //wait for done
                while (true)

                {
                    button1.Text = "Searching...";
                    button1.Enabled = false;
                    Thread.Sleep(230);

                    try
                    {
                        string[] done = File.ReadAllLines("Done.txt");
                        if (done[0] == "1")

                        {
                            button1.Text = "Search";
                            button1.Enabled = true;
                            TextWrite("Done", "0");
                            break;



                        }
                    }

                    catch
                    {
                        Thread.Sleep(25);
                        string[] done = File.ReadAllLines("Done.txt");
                        if (done[0] == "1")

                        {
                            button1.Text = "Search";
                            button1.Enabled = true;
                            TextWrite("Done", "0");
                            break;



                        }
                    }


                    

                }

                //command 4
                if (command == 1)
                {
                    command = 4;
                    TextWrite("The Word", command.ToString());
                    TextWrite("Read", "1");
                
                    //wait for done
                while (true)

                {
                        button1.Text = "Searching...";
                        button1.Enabled = false;
                        try
                    {
                        string[] done = File.ReadAllLines("Done.txt");
                        if (done[0] == "1")

                        {
                                button1.Text = "Search";
                                button1.Enabled = true;
                                TextWrite("Done","0");
                                break;



                        }
                    }

                    catch
                        {
                            Thread.Sleep(25);
                            string[] done = File.ReadAllLines("Done.txt");
                            if (done[0] == "1")

                            {
                                button1.Text = "Search";
                                button1.Enabled = true;
                                TextWrite("Done", "0");
                                break;



                            }
                        }


                    Thread.Sleep(230);

                }
            }
                if (command==2||command==3||command==4) 
                {
                    command = 3;
                    TextWrite("The Word", command.ToString()+textBox1.Text);
                    TextWrite("Read", "1");
                }
                //wait for done
                while (true)

                {
                    button1.Text = "Searching...";
                    button1.Enabled = false;
                    try
                    {
                        string[] done = File.ReadAllLines("Done.txt");
                        if (done[0] == "1")

                        {
                            button1.Text = "Search";
                            button1.Enabled = true;
                            TextWrite("Done", "0");
                            break;



                        }
                    }

                    catch
                    {
                        Thread.Sleep(25);
                        string[] done = File.ReadAllLines("Done.txt");
                        if (done[0] == "1")

                        {
                            button1.Text = "Search";
                            button1.Enabled = true;
                            TextWrite("Done", "0");
                            break;



                        }
                    }


                    Thread.Sleep(230);

                }
                //Thread.Sleep(500);
                //show results
                string[] resultFiles = File.ReadAllLines("The Result.txt");
                
                //open five files
                if (resultFiles[0] != "not found")
                {
                    label3.Visible = false;
                    label2.Visible = true;
                    label2.Text = resultFiles.Length.ToString() + " Files Contain " + "\"" + textBox1.Text + "\""  +  " !";
                    for (noOfResultFiles = 0; noOfResultFiles < 5; noOfResultFiles++)

                    {
                        if(noOfResultFiles >= resultFiles.Length) { label3.Visible = false; break; }
                        Process.Start(resultFiles[noOfResultFiles]);
                       
                    }
                }
                //no result
                else { label2.Visible = true; label2.Text = "No Files Contain " + "\"" + textBox1.Text + "\"" + " !"; label3.Visible = false; }
                //next
                if (resultFiles.Length>5) { label3.Visible=true; }


            }
           

        }

        private void Form1_Load(object sender, EventArgs e)
        {
             
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }


      

        private void textBox1_MouseClick(object sender, MouseEventArgs e)
        {
            textBox1.Text = "";
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            label2.Visible = false;
            label3.Visible = false;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            FolderBrowserDialog file = new FolderBrowserDialog();
            label2.Visible = false;
            label3.Visible = false;
            if (file.ShowDialog()==System.Windows.Forms.DialogResult.OK)
            {

                 location = file.SelectedPath+"\\";
                command = 1;
                TextWrite("The Word", "0");
                TextWrite("Read", "1");
                first = true;

            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TextWrite("The Word","0");
            TextWrite("Read", "1");
            this.Close();
           
        }

        private void label3_MouseClick(object sender, MouseEventArgs e)
        {
            //show next 5 text files
            
            string[] resultFiles = File.ReadAllLines("The Result.txt");


            int i = 0;
                while(true)

                {
              
                if (i==5) { break; }
                if (noOfResultFiles >= resultFiles.Length) { label3.Visible = false;break; }
                 Process.Start(resultFiles[noOfResultFiles]); noOfResultFiles++; i++;
            }
            
            


        }

        private void loadATreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            //if (location=="") { MessageBox.Show("Choose a file location first"); }
            label2.Visible = false;
            label3.Visible = false;
          
            FolderBrowserDialog file = new FolderBrowserDialog();

            if (file.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                // TextWrite("The Inverted Index", file.FileName );
                location = file.SelectedPath + "\\";
                command = 2;
                TextWrite("The Word", "0");
                TextWrite("Read", "1");
                first = true;

            }
        }

        private void usePreviousIndexToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (usePreviousIndexToolStripMenuItem.Checked==true) { usePreviousIndexToolStripMenuItem.Checked = false; }
            else if (usePreviousIndexToolStripMenuItem.Checked == false) { usePreviousIndexToolStripMenuItem.Checked = true; }

        }
    }
}









/* Process.Start(cPath);
                while (true)
                
                {
                    string[] resultFiles = File.ReadAllLines("The Result.txt");
                    if (resultFiles[0] != "")
                    
                    {
                        //open all files
                        for (int i = 0; i < resultFiles.Length; i++)

                        {
                            Process.Start(resultFiles[i]);
                            
                        }

                        label2.Text ="Number of Files Containing "+"\""+textBox1.Text+"\""+" is "+resultFiles.Length.ToString()+" !";
                        break;
                    
                    }
                    Thread.Sleep(250);
                
                }*/

/*   //invert slash
            char[] newLocation = new char[location.Length];
            string newLocationString = "";
            for (int i=0;i<location.Length;i++)
            {
                if (location[i] == '\\')
                { newLocation[i] = '/'; }
                else
                { newLocation[i] = location[i]; }
            }


            for (int i = 0; i < location.Length; i++) { newLocationString += newLocation[i]; }
            */
