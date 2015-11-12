window.onload = function() {

  basepath = Drupal.settings.geolocate.basepath;
  geodebug = Drupal.settings.geolocate.debug;
  // alert(geodebug);  //@DEBUG

// @TODO: Should we use Modernizr here to detect geolocation support, instead of the navigator object?
// if (Modernizr.geolocation) {
if (navigator.geolocation) {

  var options = {
    enableHighAccuracy: true,
    timeout: 5000,
    maximumAge: 0
  };

  function geoGood(pos) {
    var crd = pos.coords;
    var lat = crd.latitude;
    var lng = crd.longitude
    // crd.accuracy

    jQuery.ajax({
    url: '/' + basepath + "geo/locate",
    type: "post",
    dataType: 'json',
    data: 'lat=' + lat + '&lng=' + lng,
    success: function(data){
      if (geodebug == 1) {
        console.log(data);
        }
      }
    })
  };

  function geoError(err) {
    console.warn('ERROR(' + err.code + '): ' + err.message);
  };

  navigator.geolocation.getCurrentPosition(geoGood, geoError, options);

}
  else {
     // no native support; maybe try a fallback?
    alert('Geolocation is not supported in your browser');
  }

}
