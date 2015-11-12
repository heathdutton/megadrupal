/**
 * @file
 * This files contains all the documentation of the javascript behaviour 
 * that is being exposed.
 */

/**
 * Triggered when unread element is clicked.
 * 
 * This is triggered when element that is not read is clicked. and 
 * before Ajax Call is being made.
 */
$(document).bind("MarkAsRead.UnReadClicked", function (event, jqueryClickedElement) {
  // Do your stuffs.
});


/**
 * Triggered when user activity is saved through Ajax.
 * 
 * This is triggered when unread element is clicked and Ajax call is complete.
 */
$(document).bind("MarkAsRead.UnReadClickedAjaxComplete", function (event, jqueryClickedElement) {
  // Do your stuffs.
});
