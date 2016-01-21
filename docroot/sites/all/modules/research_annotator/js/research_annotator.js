/**
 * @file: Front-end functionality of the research annotator module.
 *        This javascript takes care of user interactions and the setting of hidden
 *        form values. Therefore it is crucial that you do not modify this file 
 *        or use javascript that conflicts with this functionality.
 */

(function ($) {
  Drupal.behaviors.researchAnnotator = {
    attach: function (context, settings) {
          
      /*
       * Helper function. 
       * Sets form variables and performs visuals based on the paragraph you want to annotate.
       * @param i_paragraphIndex: The index of the paragraph you want to annotate. Index starts at 1.
       * @param o_annotatableElements: A jQuery object of elements from the DOM.
       */
      function changeAnnotatedParagraph(i_paragraphIndex, o_annotatableElements){
        var o_annotatableElement = $(o_annotatableElements).get(i_paragraphIndex - 1);
        $(o_annotatableElement).addClass('annotatable-selected');
        $(o_annotatableElement).parent().prepend(
          Drupal.theme(
            'renderCurrentElementContainer', 
            $(o_annotatableElement).prop("outerHTML"),
            i_paragraphIndex
          )
        );
        $('input[name="paragraph_index"]').val(i_paragraphIndex);
        $('input[name="paragraph_index"]').val(i_paragraphIndex);
      }
      
      // Prepare annotatable elements to be used in our javascript.
      var s_annotatableElements = 'p, table, ul, ol';
      var o_annotatableElements = $('.annotated-body').find(s_annotatableElements);
      
      $(o_annotatableElements).each(function(i_index, m_value){
        if($(m_value).text() == String.fromCharCode(160)){
          $($(o_annotatableElements).get(i_index)).remove();
        }  
      });

      var o_annotatableElements = $('.annotated-body').find(s_annotatableElements);

      // Create some visuals on page load based on previous form submissions.
      if($('input[name="paragraph_index"]').val() !== ''){
        changeAnnotatedParagraph($('input[name="paragraph_index"]').val(), o_annotatableElements);
      }
      
      // Interacting with annotatable paragraphs.
      $(o_annotatableElements).hover(
        function(){
          $(this).before('<div class="annotate-this">' + Drupal.t('Click to annotate.') + '<div class="annotate-corner"></div></div>');
        },
        function(){
          $('.annotate-this').remove();
          $('.annotate-corner').remove();
        }
      );
      
      $(o_annotatableElements).click(function(){
        $(o_annotatableElements).removeClass('annotatable-selected');
        $(this).addClass('annotatable-selected');
        changeAnnotatedParagraph($(o_annotatableElements).index($(this).get(0)) + 1, o_annotatableElements);
        $('html, body').animate({scrollTop: $('form#annotation-creation').offset().top - 375}, 1000);
      });
      
      // Functionality for interacting with existing annotations.
      $('h3[trigger="toggle-annotation-display"]').click(function(){
        var i_index = $(this).attr('id').match(/\d+/g);
        i_index = i_index[0];
        
        if($(this).parent().hasClass('annotation-section-minimized')){
          
          $(this).parent().siblings('.annotation-section-maximized')
          .removeClass('annotation-section-maximized')
          .addClass('annotation-section-minimized');
          
          $(this).parent()
          .removeClass('annotation-section-minimized')
          .addClass('annotation-section-maximized');
          
          $(o_annotatableElements).removeClass('annotatable-selected');
          changeAnnotatedParagraph(i_index, o_annotatableElements);
        }
        else{
          
          $(this).parent().removeClass('annotation-section-maximized').addClass('annotation-section-minimized');
          $(o_annotatableElements).removeClass('annotatable-selected');
        }
      });
      
      // Closing the current annotations container.
      $('#close-current-annotation').live('click', function(e){
        e.preventDefault();
        $('#annotation-creation input[name="paragraph_index"]').val('');
        $('#annotated-header-container').remove();
      });
      
      // Visuals on annotation edit page.
      var i_paragraphIndex = $('#annotation-edit input[name="paragraph_index"]').val();
      $($('.annotation-content-edit').children(s_annotatableElements).get(i_paragraphIndex - 1)).addClass('annotatable-selected');
      $('.annotation-content-edit').children('*').not('.annotatable-selected').remove();
    }
  };

  Drupal.theme.prototype.renderCurrentElementContainer = function (s_annotatableHtml, i_paragraphIndex) {
    
    $('#annotated-header-container').remove();
    
    var s_html =  '<div id="annotated-header-container">';
    s_html +=       '<button id="close-current-annotation">' + Drupal.t('Close Current Annotation') + '</button>';
    s_html +=       '<span id="container-label">' + Drupal.t('Selected Annotation (Section @index)', {'@index': i_paragraphIndex}) + ':</span>';
    s_html +=       '<div id="annotated-header-wrapper">' + s_annotatableHtml + '</div>';
    s_html +=     '</div>';
    
    return s_html;
  };

})(jQuery);
