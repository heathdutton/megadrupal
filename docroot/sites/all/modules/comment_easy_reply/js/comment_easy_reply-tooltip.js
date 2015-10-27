/**
 * @file
 * Contains Comment Easy reply frontend javascript functions.
 */

(function ($) {
  Drupal.behaviors.comment_easy_reply_tooltip = {
    attach: function (context, settings) {
      var comment_form = $("#comment-form #edit-comment-body");
      var comment_subject_form = $("#comment-form #edit-subject");
      $('.comment-easy-reply-number-link-wrapper', context).once('comment-easy-reply-tooltip').each(function () {
        var tips = $(this).children('.comment-easy-reply-number-link-tips');
        var link = $(this).children('.comment-easy-reply-number-link');
        if (Drupal.settings.comment_easy_reply != 'undefined'
          && Drupal.settings.comment_easy_reply.replytip_activated
          && Drupal.comment_easy_reply.is_native_tooltip_enabled()) {
          link.hover(function () {
            $('.comment-easy-reply-number-link-tips').hide();
            tips.show();
            Drupal.comment_easy_reply.init_tips(link,tips);
          },
          function (e) {
            var linkY = link.offset().top + link.outerHeight();
            var posY = e.pageY;
            if (posY > linkY) {
              tips.hide();
            }
          });
          tips.hover(function () {
            tips.show();
          },
          function () {
            tips.hide();
          });
        }
      });
      $('.comment-easy-reply-referrer-link-wrapper', context).once('comment-easy-reply-tooltip').each(function () {
        $(this).find('.comment-easy-reply-number-link').each(function () {
          var commentnumber = Drupal.comment_easy_reply.get_number_from_class($(this), 'comment-easy-reply-linknum-');
          if (commentnumber != 'undefined') {
            var selector = '.comment-easy-reply-referrer-tips-' + commentnumber;
            var add_class = Drupal.comment_easy_reply.get_add_class($(this));
            if (add_class) {
              selector += '.comment-easy-reply-added-' + add_class;
            }
            else {
              selector += '.comment-easy-reply-added-noclass';
            }
            var tips = $(this).closest('.comment').find(selector);
            if (Drupal.comment_easy_reply.is_native_tooltip_enabled()) {
              $(this).hover(function () {
                $('.comment-easy-reply-number-link-tips, .comment-easy-reply-number-link-quote-tips').hide();
                tips.show();
                Drupal.comment_easy_reply.init_tips(this,tips);
              },
              function () {
                tips.hide();
              });
            }
          }
        });
      });
    }
  };
})(jQuery);
