/**
 * @author jmsirvent
 */

/**
 * Global Object for Namespace
 */
var weatherEs = weatherEs || {};
/**
 * Drupal behaviors
 */
Drupal.behaviors.weatherEs = {
  attach: function(context){
    //jQuery('.weather_es p strong').css('background-color', 'blue');
    // Get the days with her info
    var days = jQuery('.weather_es');
    // Init the object
    weatherEs.days = days;
    weatherEs.page = 1;

    weatherEs.pager();
    jQuery('a.active').click(function(){
      weatherEs.page = jQuery(this).attr('rel');
      weatherEs.pager();
    });
    //setInterval(weatherEs.periodicRefresh, 10000);
  }
};
/**
 * Handle the visibility of the diferent days
 */
weatherEs.pager = function(){
  var page = weatherEs.page;
  var days = weatherEs.days;

  days.each(function(){
    var rel = jQuery(this).attr('rel');
    if (rel != page){
      jQuery(this).hide();
      //jQuery(this).fadeOut('slow');
      //jQuery(this).slideUp('slow');
    }
    else{
      jQuery(this).show();
      //jQuery(this).fadeIn('slow');
      //jQuery(this).slideDown('slow');
    }

  });
  jQuery(".weather_es-pager-item").each(function(){
    if (parseInt(jQuery(this).children('a').attr('rel')) == page){
      jQuery(this).addClass('pager-current');
    }
    else{
    	jQuery(this).removeClass('pager-current');
    }
  })
};
/**
 * Periodical refresh (optional)
 */
weatherEs.periodicRefresh = function(){
  var days = weatherEs.days;
  var page = weatherEs.page;
  var lastPage = 4;

  weatherEs.pager();
  if (page == lastPage){
    weatherEs.page = 1;
  }
  else{
    weatherEs.page = page + 1;
  }
};
