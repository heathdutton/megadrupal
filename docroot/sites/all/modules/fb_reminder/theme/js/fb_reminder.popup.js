(function($) {

  $(function() {
    if (Drupal.settings.fbReminder) {
      var path = Drupal.settings.basePath + Drupal.settings.fbReminder.path + '?' +
        'destination=' + Drupal.settings.fbReminder.redirect;

      Drupal.CTools.Modal.show('CToolsFBReminderModal');
      $.ajax({
        type: 'POST',
        url: path,
        success: function (data) {
          $('#modal-title').html(data[1].title);
          $('#modal-content').html(data[1].output);
        },
        dataType: 'json'
      });
    }
  });

  /**
  * Provide the HTML to create the modal dialog.
  */
  Drupal.theme.CToolsFBReminderModal = function () {
    var html = '';

    html += '<div id="ctools-modal">';
    html += '  <div class="ctools-modal-content ctools-fbreminder-modal-content">';
    html += '    <table cellpadding="0" cellspacing="0" id="ctools-fbreminder-form">';
    html += '      <tr>';
    html += '        <td class="popups-c" valign="top">';
    html += '          <div class="popups-container">';
    html += '            <div class="modal-header popups-title">';
    html += '              <span id="modal-title" class="modal-title"></span>';
    html += '            </div>';
    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
    html += '          </div>';
    html += '        </td>';
    html += '      </tr>';
    html += '    </table>';
    html += '  </div>';
    html += '</div>';

    return html;
  }

})(jQuery);
