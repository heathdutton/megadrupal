(function($) {
  $(document).ready(function() {
  setTimeout(Drupal.refreshUserbar,Drupal.settings.userbar.refreshRate[0]);
});
  
Drupal.loadUserbarData =function(data) {
  //if the dock is available set its contents
  if ($('#userbar_dock').length)
    $('#userbar_dock').html(data['data']['#markup']);
  //if block is available set its contents
  if ($('#userbar_block').length)
    $('#userbar_block').html(data['data']['#markup']);
  //set the time out for next refresh
  setTimeout(Drupal.refreshUserbar,Drupal.settings.userbar.refreshRate[0]);
};
  
Drupal.refreshUserbar =function() {
  $.getJSON('/userbar/refresh',null,Drupal.loadUserbarData);
};
  
})(jQuery);
