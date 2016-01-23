/**
 * @file
 * Javascript related to the main view list.
 */
Drupal.theme.prototype.BounceConvertPopup = function() {
    var html = '';

    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div class="ctools-modal-content modal-forms-modal-content">';
    html += '    <div class="popups-container bounce-convert-custom-modal bounce-convert-campaign-' + Drupal.CTools.Modal.currentSettings.campaign_id + '">';
    html += '      <div class="modal-header popups-title">';
    html += '        <span class="popups-close close">' + Drupal.CTools.Modal.currentSettings.closeText + '</span>';
    html += '        <span id="modal-title" class="modal-title"> </span>';
    html += '        <div class="clear-block"></div>';
    html += '      </div>';
    html += '      <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
    html += '    </div>';
    html += '  </div>';
    html += '</div>';

    return html;
};
