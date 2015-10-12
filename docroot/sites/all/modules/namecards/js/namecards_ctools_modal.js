/*
 * Create modal template.
 */
Drupal.theme.prototype.CToolsNamecardsModal = function () {
  var html = '';
  html += '  <div id="ctools-modal">';
  html += '    <div class="ctools-modal-content  namecards-ctools-modal-content">'; // panels-modal-content
  html += '      <div class="modal-header">';
  html += '        <span id="modal-title" class="modal-title"> </span>';
  html += '        <a class="close" href="#">';
  html += '          ' + Drupal.CTools.Modal.currentSettings.closeText + Drupal.CTools.Modal.currentSettings.closeImage;
  html += '        </a>';
  html += '      </div>';
  html += '      <div id="modal-content" class="modal-content">';
  html += '      </div>';
  html += '    </div>';
  html += '  </div>';

  return html;
};

/* 
 * Modify "Add" contact link to enable display of page in Ctools modal.
 */
(function ($) {

Drupal.behaviors.namecardsCtoolsModal = {
  attach: function (context, settings) {
    // Add id attribute to close modal image.
    $('.namecards-ctools-modal-content a.close img', '#modalContent').attr('id', 'namecards-close-modal-img');
    // Add use modal class to "Add" contact link.
    $('a[href*="namecards/add_contact"]').each(function(index, ele) {
      var $ele = $(ele);
      // Specify context to make reattachment more efficient.
      var $context = $ele.parent();
      $ele.addClass('ctools-use-modal ctools-modal-namecards-style-edit');
      // Attach ctools modal behaviors to link.
      Drupal.behaviors.ZZCToolsModal.attach($context);
    });
  }
};

}(jQuery));