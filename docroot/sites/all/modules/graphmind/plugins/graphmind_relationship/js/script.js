// $Id$

/**
 * Scripts for handling on-the-fly node creation from Flex.
 */


/**
 * Plugin JavaScript object.
 */
var GraphmindRelationship = GraphmindRelationship || {};

/**
 * The parent nodeid.
 */
var parentNID;

/**
 * The active graphmind field id.
 */
var latestActiveObjectID;

/**
 * On load event.
 */
(function($) {
  /**
   * Popup window calls this to send results back to Flex.
   */
  GraphmindRelationship.overlayCloseRelationshipHandler = function() {
    if (
      Drupal.settings.hasOwnProperty('graphmind_relationship') &&
      Drupal.settings.graphmind_relationship.hasOwnProperty('child_nid')
    ) {
      document[latestActiveObjectID].sendCreationRequestBackToFlex(parentNID, Drupal.settings.graphmind_relationship.child_nid);
      delete Drupal.settings.graphmind_relationship.child_nid;
    }
  }


  // Overlay close event handler function bind to the event.
  $(document).bind('CToolsDetachBehaviors', GraphmindRelationship.overlayCloseRelationshipHandler);


  /**
   * Open popup window. Should be called from Flex through ExternalInterface.
   */
  GraphmindRelationship.openNodeCreation = function(parent_nid, default_node_type, entity_id, entity_vid, delta) {
    // Set global variables.
    var html_id =  'graphmind_map_' + entity_id + '_' + entity_vid + '_' + delta;
    latestActiveObjectID = html_id;
    parentNID = parent_nid;

    jQuery('.ctools-use-modal').remove();
    jQuery('.ctools-container').remove();

    var link = document.createElement('a');
    link.setAttribute('href', Drupal.settings.basePath + 'graphmind_relationship/overlay/ajax/' + parent_nid + '/' + default_node_type + '/' + entity_id);
    link.setAttribute('class', 'ctools-use-modal');

    var container = document.createElement('div');
    container.setAttribute('class', 'ctools-container');
    container.appendChild(link);

    jQuery('body').append(container);

    Drupal.attachBehaviors('.ctools-container');
    jQuery('.ctools-use-modal').trigger('click');
  }

  /**
   * Show the node page in the preview area - if exists.
   */
  GraphmindRelationship.loadNodeInBlock = function(nid) {
    // Removes the previous Ajax event from the element.
    $('#graphmindNodePreview').unbind('graphmindSelectBubble');

    var container = jQuery('#graphmindNodePreview');
    var autoloader_element_settings = {};
    if (container.length >= 1) {
      // Create element settings
      autoloader_element_settings.progress = {};
      autoloader_element_settings.url = Drupal.settings.basePath + '?q=graphmind_relationship/node_preview/' + nid;

      autoloader_element_settings.event = 'graphmindSelectBubble';

      var ajax = new Drupal.ajax('graphmindNodePreview', container, autoloader_element_settings);
      $(ajax.element).trigger('graphmindSelectBubble');
    }
  }

})(jQuery);
