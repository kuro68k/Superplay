using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Collections.ObjectModel;
using System.IO;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using Microsoft.Win32;
using System.ComponentModel;
using System.Threading;

namespace SPConfig
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : MetroWindow
	{
		private const string devname = "SUPERPLAY";
		private const string version = "1.0";

		#region combo box dictionaries

		public static Dictionary<string, int> ButtonModes = new Dictionary<string, int>
		{
			{ "Switchable (8/4/4AF)", 0 },
			{ "8 buttons", 1 },
			{ "4 buttons", 2 },
			{ "4 buttons + autofire", 3 },
			{ "16 buttons", 4 },
		};

		public static Dictionary<string, int> AFModes = new Dictionary<string, int>
		{
			{ "Low / High switchable", 0 },
			{ "High led LEDs on low", 1 },
			{ "Fixed (high only)", 2 },
		};

		public static Dictionary<string, int> RotaryModes = new Dictionary<string, int>
		{
			{ "Disabled", 0 },
			{ "Buttons (15/16)", 1 },
			{ "Point-of-View", 2 },
		};

		#endregion

		private MetroDialogSettings dlgSettings;
		private bool saved = true;
		private CheckBox[] af_checkboxes;
		public config current_cfg;


		public MainWindow()
		{
			InitializeComponent();
			this.DataContext = this;
			Closing += OnWindowClosing;

			dlgSettings = new MetroDialogSettings();
			dlgSettings.AnimateHide = false;
			dlgSettings.AnimateShow = false;

			af_checkboxes = new CheckBox[] { chkBtn1AF, chkBtn2AF, chkBtn3AF, chkBtn4AF, chkBtn5AF, chkBtn6AF, chkBtn7AF, chkBtn8AF,
											 chkBtn9AF, chkBtn10AF, chkBtn11AF, chkBtn12AF, chkBtn13AF, chkBtn14AF, chkBtn15AF, chkBtn16AF };

			this.Show();

			current_cfg = new config();
			current_cfg.LoadDefaults();

			string filename = Properties.Settings.Default.LastFileName;
			SetFilenameButton(filename);
			if (filename != "")
			{
				string failure_message;
				var new_config = config.ReadFromFile(filename, out failure_message);
				if (new_config == null)
				{
					Console.WriteLine(failure_message);
					//ShowMessage("Failed to open test.cfg", failure_message, MessageDialogStyle.Affirmative);
				}
				else
				{
					current_cfg = new_config;
					SyncUIToConfig(current_cfg);
					saved = true;
					SetFilenameButton(filename);
				}
			}
		}


		/* handle window close events */
		private void OnWindowClosing(object sender, CancelEventArgs e)
		{
			if (!saved)
			{
				e.Cancel = true;
				CancellationToken token;
				TaskScheduler uiSched = TaskScheduler.FromCurrentSynchronizationContext();
				Task.Factory.StartNew(ConfirmExitWithoutSave, token, TaskCreationOptions.None, uiSched);
			}
		}
		private async void ConfirmExitWithoutSave()
		{
			MessageDialogResult result = await this.ShowMessageAsync("Unsaved data", "The current configuration is unsaved. Close anyway?", MessageDialogStyle.AffirmativeAndNegative, dlgSettings);
			if (result == MessageDialogResult.Negative)
				return;
			Application.Current.Shutdown();
		}


		/* async message display */
		private void AsyncMessage(string title, string message, MessageDialogStyle style)
		{
			CancellationToken token;
			TaskScheduler uiSched = TaskScheduler.FromCurrentSynchronizationContext();
			Task.Factory.StartNew(() => AsyncMessageTask(title, message, style), token, TaskCreationOptions.None, uiSched);
		}
		private async void AsyncMessageTask(string title, string message, MessageDialogStyle style)
		{
			await this.ShowMessageAsync(title, message, style, dlgSettings);
		}


		private void SetFilenameButton(string filename)
		{
			if (filename == "")
				filename = "[none]";
			else
				filename = System.IO.Path.GetFileName(filename);

			if (!saved)
				filename = "* " + filename;

			if (btnFileName != null)
				btnFileName.Content = filename;
		}


		/* load and save */
		private void btnLoad_Click(object sender, RoutedEventArgs e)
		{
			OpenFileDialog dialog = new OpenFileDialog();
			dialog.Filter = "SUPERPLAY configuration files|*.spconfig|All files|*.*";
			dialog.CheckFileExists = true;
			dialog.DereferenceLinks = true;

			string filename = Properties.Settings.Default.LastFileName;
			if (filename != "")
				dialog.FileName = filename;
			else
				dialog.InitialDirectory = "%CSIDL_COMMON_DOCUMENTS%";
			if (dialog.ShowDialog() == true)
			{
				filename = dialog.FileName;
				Console.WriteLine(filename);

				// try to load file
				string failure_message;
				var new_config = config.ReadFromFile(filename, out failure_message);
				if (new_config == null)
				{
					Console.WriteLine(failure_message);
					AsyncMessage("Failed to open file", filename = "\n\n" + failure_message, MessageDialogStyle.Affirmative);
				}
				else
				{
					current_cfg = new_config;
					SyncUIToConfig(current_cfg);
					saved = true;
					SetFilenameButton(System.IO.Path.GetFileName(filename));
					Properties.Settings.Default.LastFileName = filename;
					Properties.Settings.Default.Save();
				}
			}
			else
				Console.WriteLine("User cancelled.");
		}

		private void btnSaveAs_Click(object sender, RoutedEventArgs e)
		{
			SaveFileDialog dialog = new SaveFileDialog();
			dialog.Filter = "SUPERPLAY configuration files|*.spconfig|All files|*.*";
			dialog.DefaultExt = ".spconfig";
			dialog.DereferenceLinks = true;

			string filename = Properties.Settings.Default.LastFileName;
			if (filename != "")
			{
				dialog.FileName = System.IO.Path.GetFileName(filename);
				dialog.InitialDirectory = System.IO.Path.GetDirectoryName(filename);
			}
			else
				dialog.InitialDirectory = "%CSIDL_COMMON_DOCUMENTS%";
			if (dialog.ShowDialog() == true)
			{
				filename = dialog.FileName;
				Console.WriteLine(filename);
				btnSave_Click(sender, e);
			}
			else
				Console.WriteLine("User cancelled.");
		}

		private void btnSave_Click(object sender, RoutedEventArgs e)
		{
			SyncConfigToUI(ref current_cfg);

			string filename = Properties.Settings.Default.LastFileName;
			if (filename == "")
			{
				btnSaveAs_Click(sender, e);
				return;
			}

			// try to save file
			if (!current_cfg.SaveToFile(filename))
			{
				Console.WriteLine("Failed to save " + filename);
				AsyncMessage("Failed to save file.", filename, MessageDialogStyle.Affirmative);
			}
			else
			{
				saved = true;
				SetFilenameButton(System.IO.Path.GetFileName(filename));
				Properties.Settings.Default.LastFileName = filename;
				Properties.Settings.Default.Save();
			}
		}

		private void btnProgram_Click(object sender, RoutedEventArgs e)
		{
			var eep = new eeprom.eep_config();

			SyncConfigToUI(ref current_cfg);
			eeprom.GenerateEEPROMConfig(current_cfg, out eep);
		}

	#region sync settings and UI

		// sync UI to config
		private void SyncUIToConfig(config cfg)
		{
			cmbButtonMode.SelectedIndex = cfg.buttons_mode;
			cmbAFMode.SelectedIndex = cfg.af_mode;
			cmbRotaryMode.SelectedIndex = cfg.rotary_mode;
			numAFHigh.Value = cfg.af_high_hz;
			numAFLow.Value = cfg.af_low_hz;
			chkBtn1AF.IsChecked = cfg.af_mask[0];
			chkBtn2AF.IsChecked = cfg.af_mask[1];
			chkBtn3AF.IsChecked = cfg.af_mask[2];
			chkBtn4AF.IsChecked = cfg.af_mask[3];
			chkBtn5AF.IsChecked = cfg.af_mask[4];
			chkBtn6AF.IsChecked = cfg.af_mask[5];
			chkBtn7AF.IsChecked = cfg.af_mask[6];
			chkBtn8AF.IsChecked = cfg.af_mask[7];
			chkBtn9AF.IsChecked = cfg.af_mask[8];
			chkBtn10AF.IsChecked = cfg.af_mask[9];
			chkBtn11AF.IsChecked = cfg.af_mask[10];
			chkBtn12AF.IsChecked = cfg.af_mask[11];
			chkBtn13AF.IsChecked = cfg.af_mask[12];
			chkBtn14AF.IsChecked = cfg.af_mask[13];
			chkBtn15AF.IsChecked = cfg.af_mask[14];
			chkBtn16AF.IsChecked = cfg.af_mask[15];
			chkAFSave.IsChecked = cfg.af_save_state;
			chkStartToggleMode.IsChecked = cfg.start_af_toggle_mode;
		}

		// sync config to UI
		private void SyncConfigToUI(ref config cfg)
		{
			cfg.buttons_mode = cmbButtonMode.SelectedIndex;
			cfg.af_mode = cmbAFMode.SelectedIndex;
			cfg.rotary_mode = cmbRotaryMode.SelectedIndex;
			cfg.af_high_hz = (float)numAFHigh.Value;
			cfg.af_low_hz = (float)numAFLow.Value;
			cfg.af_mask[0] = (bool)chkBtn1AF.IsChecked;
			cfg.af_mask[1] = (bool)chkBtn2AF.IsChecked;
			cfg.af_mask[2] = (bool)chkBtn3AF.IsChecked;
			cfg.af_mask[3] = (bool)chkBtn4AF.IsChecked;
			cfg.af_mask[4] = (bool)chkBtn5AF.IsChecked;
			cfg.af_mask[5] = (bool)chkBtn6AF.IsChecked;
			cfg.af_mask[6] = (bool)chkBtn7AF.IsChecked;
			cfg.af_mask[7] = (bool)chkBtn8AF.IsChecked;
			cfg.af_mask[8] = (bool)chkBtn9AF.IsChecked;
			cfg.af_mask[9] = (bool)chkBtn10AF.IsChecked;
			cfg.af_mask[10] = (bool)chkBtn11AF.IsChecked;
			cfg.af_mask[11] = (bool)chkBtn12AF.IsChecked;
			cfg.af_mask[12] = (bool)chkBtn13AF.IsChecked;
			cfg.af_mask[13] = (bool)chkBtn14AF.IsChecked;
			cfg.af_mask[14] = (bool)chkBtn15AF.IsChecked;
			cfg.af_mask[15] = (bool)chkBtn16AF.IsChecked;
			cfg.af_save_state = (bool)chkAFSave.IsChecked;
			cfg.start_af_toggle_mode = (bool)chkStartToggleMode.IsChecked;
		}

	#endregion

	#region misc UI handlers

		private void btnAbout_Click(object sender, RoutedEventArgs e)
		{
			AsyncMessage(devname + " Configuration tool",
						"© 2015 Paul Qureshi\nMain licence: GPL V3\n\n" +
						"HIDSharp © James F. Bellinger\n" +
						"mahapps.metro © Paul Jenkins",
						MessageDialogStyle.Affirmative);
		}

		// combo box change handlers
		private void cmbButtonMode_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			saved = false;
			SetFilenameButton(Properties.Settings.Default.LastFileName);

			KeyValuePair<string, int> d = (KeyValuePair<string, int>)cmbButtonMode.SelectedItem;
			if (af_checkboxes == null)
				return;

			switch(d.Value)
			{
				case 0:		// switchable
				case 1:		// 8 buttons
					for (int i = 0; i < 8; i++)
					{
						af_checkboxes[i].IsEnabled = true;
						af_checkboxes[i].FontWeight = FontWeights.Bold;
						af_checkboxes[8 + i].IsEnabled = false;
						af_checkboxes[8 + i].FontWeight = FontWeights.Normal;
					}
					break;

				case 2:		// 4 buttons
					for (int i = 0; i < 4; i++)
					{
						af_checkboxes[i].IsEnabled = true;
						af_checkboxes[i].FontWeight = FontWeights.Bold;
					}
					for (int i = 4; i < 16; i++)
					{
						af_checkboxes[i].IsEnabled = false;
						af_checkboxes[i].FontWeight = FontWeights.Normal;
					}
					break;

				case 3:		// 4 + AF
					for (int i = 0; i < 16; i++)
					{
						af_checkboxes[i].IsEnabled = false;
						af_checkboxes[i].FontWeight = FontWeights.Normal;
					}
					break;

				case 4:		// 16 buttons
					for (int i = 0; i < 16; i++)
					{
						af_checkboxes[i].IsEnabled = true;
						af_checkboxes[i].FontWeight = FontWeights.Bold;
					}
					break;
			}
		}

		private void cmbAFMode_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			saved = false;
			SetFilenameButton(Properties.Settings.Default.LastFileName);

			if (numAFLow == null)
				return;

			KeyValuePair<string, int> d = (KeyValuePair<string, int>)cmbAFMode.SelectedItem;

			switch (d.Value)
			{
				case 0:		// high/low
					numAFLow.IsEnabled = true;
					numAFLow.FontWeight = FontWeights.Bold;
					tblkAFLow.Foreground = Brushes.Black;
					break;

				case 1:		// high with LEDs
					numAFLow.IsEnabled = false;
					numAFLow.FontWeight = FontWeights.Normal;
					tblkAFLow.Foreground = Brushes.Gray;
					break;

				case 2:		// fixed (high only)
					numAFLow.IsEnabled = false;
					numAFLow.FontWeight = FontWeights.Normal;
					tblkAFLow.Foreground = Brushes.Gray;
					break;
			}
		}

		private void GenericConfigChanged(object sender, RoutedEventArgs e)
		{
			saved = false;
			SetFilenameButton(Properties.Settings.Default.LastFileName);
		}

	#endregion

		/*
		private void numAF_ValueChanged(object sender, RoutedEventArgs e)
		{
			//saved = false;
			//SetFilenameButton(Properties.Settings.Default.LastFileName);
			GenericConfigChanged(sender, e);

			NumericUpDown numeric = sender as NumericUpDown;
			double frac;
			if (numeric.Value != null)
				frac = (double)numeric.Value;
			else
				frac = 0.5;
			frac = frac - Math.Truncate(frac);
			if ((frac != 0) && (frac != 0.5))
				//numeric.Value = Math.Round((double)numeric.Value, 0);
				numeric.Value = 1;

			e.Handled = true;
		}

		private void numAF_PreviewTextInput(object sender, TextCompositionEventArgs e)
		{
			e.Handled = false;

			double num;
			if (!double.TryParse(e.Text, out num))
				e.Handled = true;
			else
			{
				double frac = num - Math.Truncate(num);
				if ((frac != 0) && (frac != 0.5))
					e.Handled = true;
			}
			
			base.OnPreviewTextInput(e);
		}
		*/
	}

	/* layout assistance */
	public class MarginSetter
	{
		public static Thickness GetMargin(DependencyObject obj)
		{
			return (Thickness)obj.GetValue(MarginProperty);
		}

		public static void SetMargin(DependencyObject obj, Thickness value)
		{
			obj.SetValue(MarginProperty, value);
		}

		// Using a DependencyProperty as the backing store for Margin.  This enables animation, styling, binding, etc...
		public static readonly DependencyProperty MarginProperty =
			DependencyProperty.RegisterAttached("Margin", typeof(Thickness), typeof(MarginSetter), new UIPropertyMetadata(new Thickness(), MarginChangedCallback));

		public static void MarginChangedCallback(object sender, DependencyPropertyChangedEventArgs e)
		{
			// Make sure this is put on a panel
			var panel = sender as Panel;
			if (panel == null) return;
			panel.Loaded += new RoutedEventHandler(panel_Loaded);
		}

		static void panel_Loaded(object sender, RoutedEventArgs e)
		{
			var panel = sender as Panel;

			// Go over the children and set margin for them:
			foreach (var child in panel.Children)
			{
				var fe = child as FrameworkElement;

				if (fe == null) continue;

				fe.Margin = MarginSetter.GetMargin(panel);
			}
		}
	}
}
