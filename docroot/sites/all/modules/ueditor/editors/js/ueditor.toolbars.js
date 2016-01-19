/**
 * ueditor.toolbars.js for Drupal7 by Drupalhunter.com
 */

(function ($) {
  Drupal.ueditor = Drupal.ueditor || {};

  Drupal.behaviors.ueditor = {
    attach: function(context, settings) {
      var toolbars = ['fullscreen', 'source', '|', 'undo', 'redo', '|',
        'bold', 'italic', 'underline', 'fontborder', 'strikethrough', 'superscript', 'subscript', 'removeformat', 'formatmatch', 'autotypeset', 'blockquote', 'pasteplain', '|', 'forecolor', 'backcolor', 'insertorderedlist', 'insertunorderedlist', 'selectall', 'cleardoc', '|',
        'rowspacingtop', 'rowspacingbottom', 'lineheight', '|',
        'customstyle', 'paragraph', 'fontfamily', 'fontsize', '|',
        'directionalityltr', 'directionalityrtl', 'indent', '|',
        'justifyleft', 'justifycenter', 'justifyright', 'justifyjustify', '|', 'touppercase', 'tolowercase', '|',
        'link', 'unlink', 'anchor', '|', 'imagenone', 'imageleft', 'imageright', 'imagecenter', '|',
        'simpleupload', 'insertimage', 'emotion', 'scrawl', 'insertvideo', 'music', 'attachment', 'map', 'gmap', 'insertframe', 'insertcode', 'webapp', 'pagebreak', 'template', 'background', '|',
        'horizontal', 'date', 'time', 'spechars', 'snapscreen', 'wordimage', '|',
        'inserttable', 'deletetable', 'insertparagraphbeforetable', 'insertrow', 'deleterow', 'insertcol', 'deletecol', 'mergecells', 'mergeright', 'mergedown', 'splittocells', 'splittorows', 'splittocols', 'charts', '|',
        'print', 'preview', 'searchreplace', 'help', 'drafts'];
      var formula = settings.ueditor.ueditor_enable_formula_editor;
      if(formula != 0){
         toolbars.push('kityformula');
      }
      Drupal.ueditor.admin_profile_form(context, settings, toolbars);
    }
  }
  
  Drupal.ueditor.render = function(element, setting) {
    if(Drupal.settings.ckeditor 
      && Drupal.settings.ckeditor.input_formats
      && Drupal.settings.ckeditor.input_formats[setting])
    return ;
    var editor = new baidu.editor.ui.Editor(Drupal.settings.ueditor.input_formats[setting]);
    editor.render(element);
  }

  Drupal.ueditor.admin_profile_form = function(context, settings, toolbars) {
    var barkey = '|';
    var barstr = '<div id="edui-toolbar" class="edui-toolbar">';    
    for(var k in toolbars) {
      barkey = toolbars[k];
      if ( barkey != '|' )
        barstr += '<div class="edui-box edui-button edui-for-'+barkey+'" id="id_'+barkey+'" title="'+barkey+'"><div class="edui-icon"></div></div>';
    }
    barstr += '</div>';    
    $('#edit-toolbars').before(barstr);
    $('.edui-icon').css({'border':'1px solid #fff'});
    $('.edui-icon').click(function(){
      var k = $(this).parent().attr('id').substr(3);
      if ( !$(this).hasClass('selected') ) {
        var s = $('#edit-toolbars').val();        
        $('#edit-toolbars').val(s+','+k);
        $(this).addClass('selected');
        $(this).css({'border':'1px solid #e22'});
      }
      else {
        var s  = $('#edit-toolbars').val().replace(','+k,'');
        $('#edit-toolbars').val(s);
        $(this).removeClass('selected');
        $(this).css({'border':'1px solid #fff'});
      }
    });    
    Drupal.ueditor.admin_profile_form_init($('#edit-toolbars').val());
  }
  
  Drupal.ueditor.admin_profile_form_init = function(v) {
    var barstr = v.split(',');
    var icodiv;
    for(var i=0; i<barstr.length; i++) {
      if(barstr[i] != '|') {
        icodiv = $('#id_'+barstr[i]+' .edui-icon');
        if(icodiv) {
          icodiv.addClass('selected');
          icodiv.css({'border':'1px solid #e22'});
        }
      }
    }
  }

}(jQuery));