/*
 * @file reel.js
 * Provides reel invokation
 * @copyright Copyright(c) 2011 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands contact at rowlandsgroup dot com
 * 
 */
(function($){
 Drupal.behaviors.reel = {
  attach: function(context) {
    $('.reel-outer img:not(.processed)').addClass('processed').reel(Drupal.settings.reel); 
  }}
})(jQuery)