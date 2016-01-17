function Echo(message)
{
   WScript.Echo(message);
}

function IsFileExists(filepath)
{
   var fso;
   fso = new ActiveXObject("Scripting.FileSystemObject");
   return (fso.FileExists(filepath));
}

function WaitForEnter()
{
   WScript.StdIn.ReadLine();
}

(function() 
{

   var FirmwareRoot, OriginalFirmware, KeyboardFirmware;

   FirmwareRoot     = "D:\\Users\\novokrest\\Github\\KeyLogging\\Arduino\\firmware";
   OriginalFirmware = FirmwareRoot + "\\Arduino-usbserial-uno.hex";
   KeyboardFirmware = FirmwareRoot + "\\Arduino-keyboard-0.3.hex";

   if (!IsFileExists(OriginalFirmware) || !IsFileExists(KeyboardFirmware)) {
      Echo("Some required firmware files don't exists");
      return;
   }

   Echo("All firmware files exist");
   Echo();

   Echo("Install original firmware '" + OriginalFirmware + "' and press Enter");
   WaitForEnter();
   Echo("Upload sketch and press Enter");
   WaitForEnter();
   Echo("Install keyboard firmware '" + KeyboardFirmware + "' and press Enter");
   WaitForEnter();

})();