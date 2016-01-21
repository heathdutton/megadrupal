(function ($) {
  Drupal.behaviors.asperaPluginInstaller = {
    attach: function (context, settings) { 

var connectInstaller = null;
var connectApplication = null;
var minConnect = "3.0.0";

// pick up the protocol and use that to download the latest connect files
// however if we are running off the local file system use http
var currentProto;
if (window.location.protocol.indexOf('file') != -1) {
	currentProto = "http:";
} else {
	currentProto = window.location.protocol;
}

var installerPath = currentProto + "//d3gcli72yxqn2z.cloudfront.net/connect/";
var installersLoaded = 0;

$.getScript(installerPath + "asperaweb-2.js", function(data, textStatus, jqxhr) { checkInstallersLoaded(); });
$.getScript(installerPath + "connectversions.js", function(data, textStatus, jqxhr) { checkInstallersLoaded(); });
$.getScript(installerPath + "connectinstaller-2.js", function(data, textStatus, jqxhr) { checkInstallersLoaded(); });

// before initAsperaConnect make sure all three installers have been loaded
var checkInstallersLoaded = function() {
installersLoaded++;
	if (installersLoaded == 3) {
	initAsperaConnect();
	}
}


var handleConnectReady = function() {
    // Called if Aspera Connect is installed and meets version requirements.
    setupConnectApplication();
	setup();
};

var handleInstallError = function() {
       // Called if an install error occurs. Display some text.
};

// Please note in a realistic real world environment you would supply a minVersion param with the startEmbeddedInstall
// The way done here will prompt for install every time (if a newer version is available), if the install is cancelled the app will still work
// if the user meets the minVersion. The way done with minVersion in start embedded install will only call an install 
// if the minVersion is not met.
var handleInstallDismiss = function() {
	//Called if install is dismissed by the user, used in connect 3.0 onwards
	setupConnectApplication();
	var connectApp = connectApplication.version().connect;
	var connectPlugin = connectApplication.version().plugin;
	
	if (connectApp.installed && connectPlugin.installed) {
		//  A version of connect is already installed, make sure it is not above our min version.
		if (versionComparison(connectApp.version, minConnect) || versionComparison(connectPlugin.version, minConnect) ) {
		disableApp('This application requires a higher version of Aspera connect (' + minVersion + '). Please upgrade in order to use this application.');
		} else {
		// Everything is good, continue to running code
		handleConnectReady();
		}  
	} else {
	// Connect is not installed
	disableApp('This application requires Aspera connect. Please install Aspera connect in order to use this application.');
	}
};

var handleInstall = function() {
   // Called if an install is required.
   connectInstaller.startEmbeddedInstall({
       installError : handleInstallError,
       stylesheet : currentProto + "//d2fvxkmjao6pcr.cloudfront.net/custom.css",
       installDismiss : handleInstallDismiss,
       prompt: true  // in a real world example you would supply minVersion here (installed of the check on handleInstallDimiss) 
   });
};


var initAsperaConnect = function () {
   // Initialize installer
   if (connectInstaller === null) {
   connectInstaller = new AW.ConnectInstaller(installerPath);
   }
   
  try {
	 connectInstaller.init({
		 connectReady : handleConnectReady,
		 install: handleInstall
	 });
  }
  catch(e){
	//catch and just suppress error
  }
   
};

// Helper function to check the version
// Assumes valid parameters will be passed i.e. 2.8.5 etc.
function versionComparison(verCur, verMin) {
	if (verCur === undefined || verMin === undefined) {
		return false;
	}
	
verCurSplit = verCur.split(".");
verMinSplit = verMin.split(".");
var less;

	if (verCurSplit[0] < verMinSplit[0]) {
		// Version is less
		less = true;
	} else if (verCurSplit[0] == verMinSplit[0]) {		
		if (verCurSplit.length > 1 && verMinSplit.length > 1) {
			// the array has another number we can look at, lets do it (remember to pass the string tho)
			less = versionComparison(verCur.slice(2), verMin.slice(2))
		} else {
			// we are assuming they are equal or the user does not care, return false
			less = false;
		}
	} else {
		// version is not less
		less = false;
	}	
	
	return less;
}

function setupConnectApplication() {
	if (connectApplication === null) {
	connectApplication = new AW.Connect({id:'aspera_web_transfers'});
	} 
}

// Helper function
function disableApp(message)
{
document.body.innerHTML = '<h1 style="text-align:center;">' + document.title + '</h1><p>' + message + '</p><p><a href="#" onclick="javascript:location.reload();return false;">Restart Connect Installer</a></p>';
}

    }};  
}) (jQuery);

