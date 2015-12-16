//$Id$

(function($) {
  $(document).ready(function(){
    $('#image-field-url-replace-button').click(function(){
      var element = $('#edit-body-und-0-value');
      if(!element.is(':visible') || element.css('visibility') == 'hidden' || element.css('opacity') == 0){
        if (Drupal.wysiwyg !== undefined) {
          $.each(Drupal.wysiwyg.instances, function() {
            element = Drupal.imageFieldUrlReplace.getElement(this.field);
            $(element).html(replacelinks($(element).html()));
          });
        }
        //element = $('.image-field-url-replace-body-1 iframe').contents().find('body');
        //$(element).html(replacelinks(element.html()));
      }
      else{
        $(element).val(replacelinks(element.val()))
      }
      return false;
    });
  });

  Drupal.imageFieldUrlReplace = {
    getElement: function(field){
      var editor = Drupal.wysiwyg.instances[field].editor;
      switch (editor) {
        // CKeditor support.
        case 'ckeditor':
          element = (CKEDITOR.instances[field].container !== undefined) ? $(CKEDITOR.instances[field].container.$).find('iframe').get(0).contentDocument : null;
          element = $(element).contents().find('body');
          break;
        // FCKeditor support.
        case 'fckeditor':
          element = (typeof(FCKeditorAPI) !== 'undefined') ? FCKeditorAPI.Instances[field].EditingArea.Document : null;
          element = $(element).contents().find('body');
          break;
        // jWYSIWYG support.
        // No insert method.
        case 'jwysiwyg':
          element = $("#" + field + "IFrame").get(0).contentDocument;
          element = $(element).contents().find('body');
          break;
        // NicEdit support.
        // No insert method.
        case 'nicedit':
          element = nicEditors.findEditor(field).elm;
          break;
        // TinyMCE support.
        case 'tinymce':
          element = (tinyMCE.editors[field] !== undefined) ? tinyMCE.editors[field].contentDocument : null;
          element = $(element).contents().find('body');
          break;
        // Whizzywig support.
        case 'whizzywig':
          element = $('#whizzy' + field).get(0).contentDocument;
          element = $(element).contents().find('body');
          break;
        // WYMEditor support.
        case 'wymeditor':
          element = $('.wym_iframe IFRAME').get(0).contentDocument;
          element = $(element).contents().find('body');
          break;
        // YUI editor support.
        case 'yui':
          element = (YAHOO.widget.EditorInfo._instances[field]._getDoc() !== false) ? YAHOO.widget.EditorInfo._instances[field]._getDoc() : null;
          element = $(element).contents().find('body');
          break;
        case 'openwysiwyg':
          element = WYSIWYG.getEditor(field);
          element = $(element).contents().find('body');
          break;
        case 'none':
          element = $('#edit-body-und-0-value');
          break;
      }
      return element;
    }
  }

  function replacelinks(val){
    //since we're going to edit the val variable, we still need the original value in a variable.
    var orig_val = val;

    var re_src = /src="(.*?)"/i;
    var matches = new Array();
    var i = 0;
    var m = re_src.exec(val)

    //as long as we find matches.
    while(m){
      matches[i] = m[1];
      i++;
      //replace the match with an empty string or we create an infinite loop.
      val = val.replace(re_src, "");
      //store the new match into m
      m = re_src.exec(val)
    }
    
    for(i in matches){
      var absolute = "http://";
      var on_server = "/sites/default";
      var on_server2 = "/sites/all";
      var file = '';

      //if the url does not start with "sites/all" and it isn't relative.
      if((matches[i].substr(0, absolute.length) != absolute) && (matches[i].substr(0, on_server.length) != on_server) && (matches[i].substr(0, on_server2.length) != on_server2)){
        file = matches[i].substr(matches[i].lastIndexOf("/")+1, matches[i].length);
      }
      //replace the image sources
      $(".image-widget-data a").each(function(){
        if($(this).text() == file){
          orig_val = orig_val.replace(matches[i], $(this).attr("href"));
        }
      })
    }
    //return the replacement value
    return orig_val;
  }



})(jQuery);

