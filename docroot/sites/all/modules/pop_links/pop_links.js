(function($){

  var PopLinksTrackClick = function() {
    // an external link has been clicked
    var args = {};
    args.operation = "track_click";
    // get the URL of the site the user is going to
    args.url = $(this).attr('href');
    // get the id of the node that contains the clicked link
    args.nid = $(this).data('nid');
    // call the popular links AJAX handler
    var rsp = $.ajax({
      data    : args,
      url     : Drupal.settings.basePath + "pop_links/handle",
      type    : "get",
      datatype: "html",
      async   : false // make sure vote is cast before sending user on her merry way
    }).responseText;
    return true;
  }

  Drupal.behaviors.pop_links = {};
  
  Drupal.behaviors.pop_links.attach = function(context, settings) {
    // Strip the host name down, removing subdomains or www.
    var host = window.location.host.replace(/^(([^\/]+?\.)*)([^\.]{4,})((\.[a-z]{1,4})*)$/, '$3$4');

    // Build regular expressions that define an internal link.
    var internal_link = new RegExp("^https?://([^/]*)?" + host, "i");

    if (Drupal.settings.hasOwnProperty('pop_links')) {
      for (var selector in Drupal.settings.pop_links) {
        var nid = Drupal.settings.pop_links[selector].nid;
        $(selector).find('a:not(.pop_links-processed)').each(function(el) {
          try{
            var url = this.href.toLowerCase();
            if (url.indexOf('http') == 0 && !url.match(internal_link)) {
              $(this).data('nid', nid);
              $(this).click(PopLinksTrackClick);
            }
          }
          // IE7 throws errors often when dealing with irregular links, such as:
          // <a href="node/10"></a> Empty tags.
          // <a href="http://user:pass@example.com">example</a> User:pass syntax.
          catch(error) {
            return false;
          }
        }).addClass('pop_links-processed');
      }
    }
  };

})(jQuery);