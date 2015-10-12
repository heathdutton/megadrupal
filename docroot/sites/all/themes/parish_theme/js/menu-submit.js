(function ($) {
  $(document).ready(function() {
    // If needed, we can set specific forms to which this behavior applies.
    var forms = $("#edit-submit");

    // Insert 'saving' div now to cache it for better performance and to show the loading image.
    $('<div id="saving"><p class="saving">Saving&hellip;</p></div>').insertAfter(forms);

    forms.click(function() {
      $(this).siblings("input:submit").hide();
      $(this).hide();
      $("#saving").show();

      var notice = function() {
        $('<p id="saving-notice">Not saving? Wait a few seconds, reload this page, and try again. Every now and then the internet hiccups too :-)</p>').appendTo("#saving").fadeIn();
      };

      // Append notice if form saving isn't work, perhaps a timeout issue.
      setTimeout(notice, 90000);
    });

    // Give .active class to main site menu parents when hovering over children.
    var menu = $("#menu ul");
    menu.each(function(i) {
      $(this).hover(function() {
        $(this).parent().parent().find("a").slice(0,1).addClass("hover-state");
      },function() {
        $(this).parent().parent().find("a").slice(0,1).removeClass("hover-state");
      });
    });
  });
})(jQuery)
