/**
 * @file
 * Contains Comment Easy Reply backend javascript functions.
 */

(function ($) {
  Drupal.behaviors.comment_easy_reply_admin = {
    attach: function (context, settings) {
      if (settings.comment_easy_reply != 'undefined'
        && settings.comment_easy_reply.admin != 'undefined'
        && settings.comment_easy_reply.admin.node_type != 'undefined') {
        var node_type = settings.comment_easy_reply.admin.node_type;
        $('#edit-comment-easy-reply-active-' + node_type).change(
          function() {
            if ($(this).attr('checked')) {
              $('#edit-comment-easy-reply #edit-override').removeClass('collapsed');
            }
            else {
              $('#edit-comment-easy-reply #edit-override').addClass('collapsed');
            }
          }
        );
        $('#edit-comment-easy-reply-override-' + node_type).change(
          function() {
            if ($(this).attr('checked')) {
              $('#edit-comment-easy-reply #edit-override #edit-active').removeClass('collapsed');
            }
            else {
              $('#edit-comment-easy-reply #edit-override #edit-active').addClass('collapsed');
            }
          }
        );
      }
      $('fieldset.comment-node-type-settings-form', context).drupalSetSummary(function(context) {
        var summary = jQuery('fieldset.comment-node-type-settings-form').data('verticalTab').summary;
        var active = 'disabled';
        if ($('#edit-comment-easy-reply-active-article').is(':checked')) {
          var active = 'enabled';
          if ($('#edit-comment-easy-reply-override-article').is(':checked')) {
            active = active + ' & overridden';
          }
        }
        var string = summary.text().replace(/(Comment Easy Reply [^,]+)*/g, "");
        return string + '<br />' + Drupal.t('Comment Easy Reply !status', {'!status': active});
      });
    }
  };
})(jQuery);
