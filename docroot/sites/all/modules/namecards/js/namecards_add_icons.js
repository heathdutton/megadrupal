/**
 * @file
 * 
 * 
 */
(function ($) {

Drupal.behaviors.NamecardsAddIcons = {
  attach: function(context, settings) {
    $('span[class*=namecards-icon-]').each(function(index) {
  	   var element = $(this);
  	   var elementClass = element.attr('class');
  	   var objRegExp;
  	   var uid;
  	   var publiclyViewable;
  	   var newClassString;
  	   var temp;
  	
  	   // Extract author uid and public/private settings from class name.
  	   objRegExp = 'namecards-icon-[0-9]{1,}-[Pp]ublic|namecards-icon-[0-9]{1,}-[Pp]rivate';
  	   //objRegExp = 'namecards-icon-[0-9]{1,}-[Pp](ublic|rivate)';
  	   temp = elementClass.match(objRegExp);
  	   temp = String(temp).match('[0-9]{1,}-[Pp]ublic|[0-9]{1,}-[Pp]rivate');
      temp = String(temp).split('-');
      
      uid = temp[0];
      
      if (temp[1].toLowerCase() == 'public') {
        publiclyViewable = 1;
      }
      else {
        publiclyViewable = 0;
      }
      
      // Creates class string based on uid of author and public/privacy 
      // settings of node.  These class names are used to set the correct icon 
      // via css (see entries for these class names in file 'namecards.css')
      if (uid == Drupal.settings.namecards.uid) {
        if (publiclyViewable == 1) {
      	newClassString = 'namecards-icon-own-public';
        }
        else {
      	newClassString = 'namecards-icon-own-private';
        }
      }
      else {
        if (publiclyViewable == 1) {
          newClassString = 'namecards-icon-other-public';
        }
        else {
        	newClassString = 'namecards-icon-other-private';
        }
      }
      
      // Add class to element. 
      element.addClass(newClassString);
    });
  }
};

}(jQuery));