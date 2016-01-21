(function ($) {
  Drupal.bootstrap = Drupal.bootstrap || {};

  Drupal.bootstrap.classSelectors = [];

  Drupal.behaviors.bootstrapAdmin = {
    attach: function(context) {
      // Show/hide layout manager button
      $('input#panels-bootstrap-toggle-layout:not(.panels-bootstrap-processed)', context)
        .addClass('panels-bootstrap-processed')
        .click(function() {
          $('.panel-bootstrap-admin')
            .toggleClass('panel-bootstrap-no-edit-layout')
            .toggleClass('panel-bootstrap-edit-layout');

          if ($('.panel-bootstrap-admin').hasClass('panel-bootstrap-edit-layout')) {
            $(this).val(Drupal.t('Hide layout designer'));
          }
          else {
            $(this).val(Drupal.t('Show layout designer'));
          }
          return false;
        });

      // Show/hide Define column size button
      $('.bootstrap-class-selector button.size-toggle').click(function() {
        $(this).toggleClass('active');        
        $(this).parent().find('.size-selector').toggleClass('hide active');
        $(this).parent().find('.ordering-selector').addClass('hide');
        $(this).parent().find('.offset-selector').addClass('hide');        
        $(this).parent().find('.ordering-toggle').removeClass('active');
        $(this).parent().find('.offset-toggle').removeClass('active');                  
      });

      // Show/hide Define column ordering button
      $('.bootstrap-class-selector button.ordering-toggle').click(function() {
        $(this).toggleClass('active');
        $(this).parent().find('.ordering-selector').toggleClass('hide');
        $(this).parent().find('.size-selector').addClass('hide');
        $(this).parent().find('.offset-selector').addClass('hide');        
        $(this).parent().find('.size-toggle').removeClass('active');
        $(this).parent().find('.offset-toggle').removeClass('active');                                              
      });

      // Show/hide Define column offset button
      $('.bootstrap-class-selector button.offset-toggle').click(function() {
        $(this).toggleClass('active');
        $(this).parent().find('.offset-selector').toggleClass('hide');
        $(this).parent().find('.size-selector').addClass('hide');
        $(this).parent().find('.ordering-selector').addClass('hide');        
        $(this).parent().find('.size-toggle').removeClass('active');
        $(this).parent().find('.ordering-toggle').removeClass('active');                                             
      });
      
      // Window splitter behavior.
      $('div.bootstrap-class-selector:not(.bootstrap-class-selector-processed)')
        .addClass('bootstrap-class-selector-processed')
        .each(function() {
          Drupal.bootstrap.classSelectors.push(new Drupal.bootstrap.classSelector($(this).find('select')));
        });
    }
  };


  Drupal.bootstrap.classSelector = function($classSelector) {
    var $this = this;
    $classSelector.each(function(){
      var $selectorFor = $("#"+$(this).attr('data-for'));
      var screenSize = $(this).attr('data-screen-size');
      var dataType = $(this).attr('data-type');
      $(this).bind('change',function(){
        var dataId = $(this).attr('data-for-id');
        var bootstrapClass = '';        
        Drupal.bootstrap.removeClasses($selectorFor,screenSize,dataType);
        $selectorFor.find("select[data-screen-size='"+screenSize+"']").each(function() {
          if(dataId == $(this).attr('data-for-id')) {
            bootstrapClass += $(this).val()+' ';
          }
        });
        $selectorFor.addClass($(this).val());
        bootstrapClass = bootstrapClass.trim();
        Drupal.ajax['bootstrap-class-selector-ajax'].options.data = {
          'item': $(this).attr('data-for-id'),
          'bootstrap_class': bootstrapClass,
          'bootstrap_screen': $(this).attr('data-screen-size')
        };
        jQuery('.panel-bootstrap-edit-layout').trigger('updateBootstrapClass');
      });
    })
  }

  Drupal.bootstrap.removeClasses = function($row,screenSize,dataType){
      $row.removeClass(function() { /* Matches even table-col-row */
        var classesToRemove = '',
        classes = this.className.split(' ');
        switch(dataType) {
            case 'size':
              regex = new RegExp('col-'+screenSize+'-[0-9]');
              break;
            case 'ordering':
              regex = new RegExp('col-'+screenSize+'-pu');
              break;
            case 'offset':
              regex = new RegExp('col-'+screenSize+'-offset');
              break;
        }        
        for(var i = 0; i < classes.length; i++ ) {
          if(regex.test(classes[i])) { /* Filters */
            classesToRemove += classes[i] + ' ';
          }
        }
        return classesToRemove ; /* Returns all classes to be removed */
    });
  }

  $(function() {

    // Create a generic ajax callback for use with the splitters which
    // background AJAX to store their data.
    var element_settings = {
      url: Drupal.settings.bootstrap.resize,
      event: 'updateBootstrapClass',
      keypress: false,
      // No throbber at all.
      progress: { 'type': 'throbber' }
    };

    Drupal.ajax['bootstrap-class-selector-ajax'] = new Drupal.ajax('bootstrap-class-selector-ajax', $('.panel-bootstrap-admin').get(0), element_settings);

    // Prevent ajax goo from doing odd things to our layout.
    Drupal.ajax['bootstrap-class-selector-ajax'].beforeSend = function() { };
    Drupal.ajax['bootstrap-class-selector-ajax'].beforeSerialize = function() { };

  });

})(jQuery);
