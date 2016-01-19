(function (window, $, JS, AR) {

  // Hey Mark, sooner or later you'll take a look at this.
  // I just wanted to say I'm sorry...
  Drupal.behaviors.ad = {
    attach: function (context, settings) {
      $(document.body).once('ad-load-ads', function () {
        postData = {};
        postData['ads'] = {};
        $("ad").each(function () {
          postData['ads'][$(this).attr('id')] = {
            view: $(this).attr('view'),
            display: $(this).attr('display'),
            arguments: $(this).attr('arguments')
          };
        });
        postData['page'] = {
          url: document.URL,
          title: document.title
        }
        postData['uid'] = Drupal.settings.ad.UserID;
        var url = Drupal.settings.basePath + Drupal.settings.ad.ServePath;
        jQuery.post(url, postData, function(data) {
          for (var element_id in data) {
            var ad = data[element_id];
            $('#' + element_id).html(ad["rendered_ad"]);
          }
        }, "json");
      });
    }
  };

})(window, window.jQuery);
