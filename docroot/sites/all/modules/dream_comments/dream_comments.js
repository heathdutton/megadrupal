(function (Drupal, $) {
  'use strict';

  Drupal.behaviors.dreamPermissions = {
    attach: function (context, settings) {
      $('.dream-comments--form').once('dream-comments', function () {
        var $form = $('.dream-comments--form');
        var $actions = $form.find('.form-actions');
        var $submit = $form.find('#edit-submit');
        var $preview = $form.find('#edit-preview');
        var $template = $form.find('.dream-comments--template');
        var $spinner = $('<div class="spinner">' + Drupal.t('Please wait') + '</div>');
        var $statusMessage = $('<div class="status-message"></div>');

        $actions.append($spinner);
        $actions.append($statusMessage);

        $submit.bind('click', function (event) {
          event.preventDefault();
          var name = $form.find('#edit-name') ? $form.find('#edit-name').val() : '';
          var mail = $form.find('#edit-mail') ? $form.find('#edit-mail').val() : '';
          var subject = $form.find('#edit-subject') ? $form.find('#edit-subject').val() : '';
          var body = $form.find('[name^="comment_body["]') ? $form.find('[name^="comment_body["]').val() : '';

          // Render temporary comment before .comments__form-title.
          var new_comment = $template.html();
          new_comment = new_comment.replace('{Name}', name);
          new_comment = new_comment.replace('{Mail}', mail);
          new_comment = new_comment.replace('{Subject}', subject);
          new_comment = new_comment.replace('{Body}', body);

          $('.comments__form-title').before(new_comment);
          $spinner.addClass('active');
          $statusMessage.text(Drupal.t('Saving comment ...'));

          var url = $form.attr('action');
          // Issue a post request.
          $.ajax({
            type: 'post',
            url: url,
            data: $form.serialize(),
            error: function (jqXHR, textStatus, errorThrown) {
              $spinner.removeClass('active');
              $statusMessage.text(Drupal.t('Something went wrong while saving ...'));
            },
            success: function (data) {
              $spinner.removeClass('active');
              $statusMessage.text(Drupal.t('Comment saved.'));
              // Use ajax to fetch the rendered comment?
            }
          });
        });

      });
    }
  }
})(Drupal, jQuery);
