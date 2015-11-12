Modernizr.addTest('pr-java', function() {
  var java = {
    hasJava: false,
    versions: []
  };
  
  if (!!navigator.javaEnabled) {
    if (!!navigator.javaEnabled() && typeof deployJava != 'undefined') {
      java.versions = deployJava.getJREs();
      
      if (java.versions.length > 0) {
        java.hasJava = true;
      }
    }
  }
  
  return java;
});