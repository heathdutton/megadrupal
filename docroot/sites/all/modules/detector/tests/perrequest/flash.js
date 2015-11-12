Modernizr.addTest('pr-flash', function() {
  var flash = {
    hasFlash: false,
    versions: []
  };
  
  if (typeof swfobject != 'undefined') {
    var version = swfobject.getFlashPlayerVersion();
    if (version.major > 0 && version.minor > 0 && version.release > 0) {
      flash.hasFlash = true;
      flash.versions.push([
        version.major, 
        version.minor, 
        version.release
      ].join('.'));
    }
  }
  
  return flash;
});