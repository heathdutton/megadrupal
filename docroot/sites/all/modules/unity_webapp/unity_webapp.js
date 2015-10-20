unityApiFuncGenerator = function (akey) {
  return function() {
    document.location.href = Drupal.settings.unity_api.links[akey].href;
  }
};

function unityReady() {
  var Unity = external.getUnityObject(1.0);
  for (key = 0; key < Drupal.settings.unity_api.links.length; key++) {
    var self = this;
    var yourloc = "/" + Drupal.settings.unity_api.links[key].href;
    Unity.addAction("/" + Drupal.settings.unity_api.links[key].name, unityApiFuncGenerator(key));
  }
}

if (external && external.getUnityObject) {
  jQuery(window).load(function(){
      var Unity = external.getUnityObject(1.0);
      Unity.init({
        name: Drupal.settings.unity_api.sitename,
        iconUrl: Drupal.settings.unity_api.favicon,
        onInit: unityReady,
        domain: Drupal.settings.unity_api.baseurl
      });
  });
}
