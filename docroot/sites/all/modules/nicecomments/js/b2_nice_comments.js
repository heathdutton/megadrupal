(function ($) {

Drupal.behaviors.b2NiceComments = {
  attach: function(context) {

    $('.b2-nice-comment-textarea').click(function() {
      $(this)
        // Hide the textarea.
        .hide('fast')
        // Get the corresponding container.
        .siblings('.b2-nice-comment-container')
        .show('fast', function() {
          var container = $('body');
          // Scroll to the comment form.
          container.scrollTop(
            $(this).find('#comment-form').offset().top - container.offset().top
          );

        })
        // Move focus to the comment body.
        .find('textarea[name*="comment_body"]')
          .focus()
        .end();
    });

    // If user wants to go directly to comment form or to reply to a form show comment form immediately
    if (location.href.indexOf('#comment-form') != -1 || location.href.indexOf('comment/reply') != -1) {
      $('.b2-nice-comment-textarea').click();
    }

  }
};

})(jQuery);
