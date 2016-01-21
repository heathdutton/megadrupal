/**
 * JavaScript behaviors for the front-end display and handling
 * 
 * Note: Handling of the counts for page/nid views must occur entirely
 *       in the client side, otherwise varnished may skip caching of
 *       the page if there are server side things being changed,
 *       for example the use of 'setcookie' etc.
 */


(function ($) {

  Drupal.behaviors.limited_views = Drupal.behaviors.limited_views || {};
  Drupal.limited_views = Drupal.limited_views || {};

  var expiry;
  var expirydays;
  var threshold;
  var count;

  // this is triggered from the drupal_add_js in the module
  // so we can allow other ways to use this functionality  
  Drupal.limited_views.default_trigger = function() {
    threshold = parseInt(Drupal.settings.limited_views.threshold); 
    expiry = parseInt(Drupal.settings.limited_views.expiry); // seconds to days
    expirydays = parseInt(expiry / 60 / 60 / 24); // seconds to days
    
    count = Drupal.limited_views.get_view_count();  
    if (count <= threshold) {
      Drupal.limited_views.add(Drupal.settings.limited_views.nid);
    }
    if (count >= threshold - 1) { // second from last allowed view
      if (!$.cookie('NO_CACHE')) {
        var d = new Date();
        d.setMinutes(d.getMinutes() + 2);
        // Set the NO_CACHE cookie so that the next page load uses the PHP function to check.
        $.cookie('NO_CACHE', 'Y', { expires: d, path: '/' }); 
      }
    }
  };

  Drupal.limited_views.get_view_count = function() {

    var expiry = parseInt(Drupal.settings.limited_views.expiry); // seconds to days  
    var expiry_stamp = Math.floor(new Date().getTime()/1000) - expiry;

    var count = 0;
    if ($.cookie(Drupal.settings.limited_views.cookie_name_count)) {
      var nodeobj = JSON.parse($.cookie(Drupal.settings.limited_views.cookie_name_count)); // cookie containing object
      if (nodeobj != null) {
        $.each(nodeobj, function(index, value) {
          if (value['timestamp'] > expiry_stamp) {
            count ++;
          }
        });
      }
    }
    return count;
  };

  /**  
   * See if this nid exists in our history
   */
   
  Drupal.limited_views.exists = function(nid) {
    var exists = false;
    var nodeobj = new Array();
    var i = 0;
    threshold = parseInt(Drupal.settings.limited_views.threshold); 

    nid = parseInt(nid); // ensure it's an integer
    if ($.cookie(Drupal.settings.limited_views.cookie_name_count)) {
      var cookie = JSON.parse($.cookie(Drupal.settings.limited_views.cookie_name_count));
      if($.isArray(cookie)) {
        nodeobj = cookie;
        // check if this node is a new one
        $.each(nodeobj, function(index, value) {
          i =i +1;
          // the item that is outside of the threshold isnt included
          // this is jsut to test if the nid exists inside of our threshold
          // number of views
          if (value['nid'] == nid && i <= threshold) {
            exists = true;
          }
        });
      } 
    } 
   return exists;
  };
    

  /**
   * Adds cookies to track number of node views
   */
  Drupal.limited_views.add = function(nid) {

    var timestamp = Math.floor(new Date().getTime()/1000);
    var newnid = true;
    nid = parseInt(nid); // ensure it's an integer

    if ($.cookie(Drupal.settings.limited_views.cookie_name_count)) {
      var cookie = JSON.parse($.cookie(Drupal.settings.limited_views.cookie_name_count));
    } else {
      cookie = new Array();
    }

    if( ! Drupal.limited_views.exists(nid) ) {
      // record this nid as viewed
        viewInstance = new Object();
        viewInstance.nid = nid;
        viewInstance.timestamp = timestamp;
        cookie.push(viewInstance);
        $.cookie(Drupal.settings.limited_views.cookie_name_count, JSON.stringify(cookie), {
            expires : expirydays,
            path : '/'
        });
    }
    
    return newnid;
  };
  
})(jQuery);


