/**
 * @file
 * Contains Comment Easy Node reply frontend javascript functions.
 */

(function ($) {
  Drupal.behaviors.comment_easy_reply_node = {
    attach: function (context, settings) {     
      var comment_form = $("#comment-form #edit-comment-body");
      var comment_subject_form = $("#comment-form #edit-subject");
      $('.comment-easy-reply-node-first-reply', context).once('comment-easy-reply').each(function () {
        var link = $(this);
        if (Drupal.settings.comment_easy_reply != 'undefined'
          && comment_form.length > 0
          && Drupal.settings.comment_easy_reply.user_can_reply) {
          link.click(function (e) {
            var throbber = $(Drupal.theme('comment_easy_reply_throbber'));
            link.append(throbber);
            var nodenumber = Drupal.comment_easy_reply.get_number_from_class(this, 'comment-easy-reply-node-node-');
            var url = Drupal.comment_easy_reply.get_full_url('ajax/comment-easy-reply/node/' + nodenumber + '/first-reply');
            var scrollto = false;
            if (Drupal.settings.comment_easy_reply.scrollform_enabled) {
              scrollto = true;
            }
            $.getJSON(
             url,
             {
               format: "json"
             },
             function(data) {
               Drupal.comment_easy_reply.insert_title(data.comment.subject, false);
               Drupal.comment_easy_reply.insert_content(data.comment.body + ' ');
               if(scrollto){
                 Drupal.comment_easy_reply.scroll_to_comment_form();
               }
               throbber.remove();
             }
            );
            e.preventDefault();
          });
        }
      });
    }
  };
})(jQuery);