
/* QueryBuilder class */
function QueryBuilder(options)
{
  /** Self reference for total closure */
  var self = this;  
  
  /* base URL of the drupal instance to use to proxy all the queries to structWSF */
  this.drupal = options.proxy.replace(/\/$/,'')+'/';

  this.project_path = options.project_path;
  
  /* structWSF network to query */
  this.network = options.network.replace(/\/$/,'')+'/';
  
  this.schemas = options.schemas;
  this.properties = {};
  this.properties['dataset'] = {
    type: 'http://www.w3.org/2002/07/owl#ObjectProperty', 
    prefLabel: 'dataset',
    isObjectProperty: true
  };
  this.classes = {};
  
  this.excludeDatasets = options.exclude_datasets;
  
  this.namespaces = options.namespaces;
  
  this.datasets = [];
  this.datasetsCounts = [];
  this.excludedDatasets = [];
  this.propertiesCounts = [{uri: 'dataset', count: -1}];
  this.classesCounts = [];
  this.initialized = false;
  this.currentPage = 1;
  this.inferenceEnabled = true;
  this.initialProperties = [];
  this.useInterface = 'DefaultSourceInterface';
  this.structWSFPHPAPIQuery = '';
  this.jsonQuery = {};
  
  this.propertiesLabelsFixes = [
    {
      uri: 'http://www.w3.org/1999/02/22-rdf-syntax-ns#value',
      label: 'content body',
      type: 'http://www.w3.org/2002/07/owl#DatatypeProperty'
    }
  ]
  
  this.init = function init()
  {
    /* Initalize the excludedDatasets from the cache */
    if(jQuery.cookie('qb-excluded-datasets') != null)
    {
      this.excludedDatasets = JSON.parse(jQuery.cookie('qb-excluded-datasets'));
    }
    
    /* Close all accordion panels when we start the Query Builder */
    jQuery( "#accordion" ).accordion({
        active: false,
      });     
    
    /* Initialize contextual help buttons */
    jQuery('#help-search-profile-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#search-profile-modal").load(self.project_path + '/doc/qb_search_profiles.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Search Profile Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });    
    
    jQuery('#help-advanced-search-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-advanced-search-modal").load(self.project_path + '/doc/qb_advanced_search.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Advanced Search Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });
    
    jQuery('#help-search-restrictions-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-search-restrictions-modal").load(self.project_path + '/doc/qb_attributes.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Attributes Restrictions &amp; Boosting Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });    
        
    jQuery('#help-score-boosting-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-score-boosting-modal").load(self.project_path + '/doc/qb_values.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Value Boosting Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });  
    
    jQuery('#help-results-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-results-modal").load(self.project_path + '/doc/qb_results.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Results Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });  
    
    jQuery('#help-search-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-search-modal").load(self.project_path + '/doc/qb_main.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Search',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });      
                  
    jQuery('#help-phrase-boosting-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-phrase-boosting-modal").load(self.project_path + '/doc/qb_phrases.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Search Phrase Treatment Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });    
        
    jQuery('#help-search-settings-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#search-settings-modal").load(self.project_path + '/doc/qb_osf_for_drupal_settings.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - OSF for Drupal Search Settings Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });               
            
    jQuery('#help-search-query-code-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#search-query-code-modal").load(self.project_path + '/doc/qb_query_code.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Search Query Code Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });        
                  
    jQuery('#help-other-options-button').button({
       text: false,
       icons: {
           primary: "ui-icon-info"
       }
    }).click(function(){
      jQuery("#help-other-options-modal").load(self.project_path + '/doc/qb_operators.php?path=' + escape(self.project_path)).dialog({
        title: 'Help - Search Other Options Section',
        modal: true,
        minWidth: jQuery(window).width() * 0.7,
        minHeight: jQuery(window).height() * 0.7,
        maxWidth: jQuery(window).width() * 0.7,
        maxHeight: jQuery(window).height() * 0.7
      });   
    });    
    
    
    
    /* Skin the search input */
    jQuery('#search-input')
      .button()
      .css({
              'font' : 'inherit',
             'color' : 'inherit',
        'text-align' : 'left',
           'outline' : 'none',
            'cursor' : 'text',
            'width'  : '96%'
      })
      .unbind()
      .keypress(function(e) {
        if(e.which == 13) {
            jQuery(this).blur();
            self.search();
        }
    });
    
    jQuery('#clear-button-top').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    jQuery('#clear-button-adv-section').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    jQuery('#clear-button-restriction-section').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    jQuery('#clear-button-score-section').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    jQuery('#clear-button-phrase-section').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    jQuery('#clear-button-other-options-section').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    jQuery('#clear-button-query-code-section').click(function(event){
      event.preventDefault();
      location.reload(); 
    });
    
    /* Initialize the inference button */
    jQuery('#inference-icon').button().click(function(){
      if(self.inferenceEnabled)
      {
        self.inferenceEnabled = false;
        jQuery(this).attr('src', self.project_path + '/css/ui-lightness/images/cog_add.png');
        jQuery(this).attr('title', 'Enable inference');
        self.search();
      }
      else
      {
        self.inferenceEnabled = true;
        jQuery(this).attr('src', self.project_path + '/css/ui-lightness/images/cog_delete.png');
        jQuery(this).attr('title', 'Disable inference');
        self.search();
      }
    });
    
    
    /* Consolidate properties and classes across all schemas */
    for(var i = 0; i < this.schemas.length; i++)
    {
      for(var uri in this.schemas[i].attributes)
      {
        if(!this.properties.hasOwnProperty(uri))
        {
          this.properties[uri] = this.schemas[i].attributes[uri];
          
          for(var p = 0; p < this.propertiesLabelsFixes.length; p++)
          {
            if(this.propertiesLabelsFixes[p].uri == uri)
            {
              this.properties[uri].prefLabel = this.propertiesLabelsFixes[p].label;
              this.properties[uri].type = this.propertiesLabelsFixes[p].type;
              break;
            }
          }
        }
      }    
      
      for(var uri in this.schemas[i].types)
      { 
        if(this.classes[uri] == undefined)
        {
          this.classes[uri] = this.schemas[i].types[uri];         
        }
        else
        {
          if(this.schemas[i].types[uri].subTypeOf != undefined &&
             this.classes[uri].subTypeOf != undefined)
          {
            for(var t = 0; t < this.schemas[i].types[uri].subTypeOf.length; t++)
            {
              var found = false;
              
              for(var tt = 0; tt < this.classes[uri].subTypeOf.length; tt++)
              {
                if(this.schemas[i].types[uri].subTypeOf[t] == this.classes[uri].subTypeOf[tt])
                {
                  found = true;
                  break;
                }
              }
              
              if(!found)
              {
                this.classes[uri].subTypeOf.push(this.schemas[i].types[uri].subTypeOf[t]);
              }
            }
          }
        }
      }    
    }
    
    /* Tag properties has been datatype or object properties */
    for(var uri in this.properties)
    {
      var iop = this.isObjectProperty(this.properties[uri]);
      
      if(iop)
      {
        this.properties[uri]['isObjectProperty'] = true;
      }
      else
      {
        this.properties[uri]['isObjectProperty'] = false;
      }
    }
    
    /* Make sure that if two properties have the same preflabel that we
       disambiguate them with the provenance of both */
       
    for(var uriFirst in this.properties)
    {
      for(var uriSecond in this.properties)
      {
        if(uriFirst != uriSecond &&
           this.properties[uriFirst]['prefLabel'] == this.properties[uriSecond]['prefLabel'])
        {
          this.properties[uriFirst]['prefLabel'] += (this.getNamespacePrefix(uriFirst) == '' ? '' : ' ('+this.getNamespacePrefix(uriFirst)+')');
          this.properties[uriSecond]['prefLabel'] += (this.getNamespacePrefix(uriSecond) == '' ? '' : ' ('+this.getNamespacePrefix(uriSecond)+')');
        }
      }      
    }
    
    /* Create the Advanced Search section */
    jQuery('#advanced-search-section > p').append('<table id="advanced-search-filter-table">\
                                                <tbody>\
                                                  <tr class="advanced-search-filter-row">\
                                                    <td class="advanced-search-col-1"></td>\
                                                    <td class="advanced-search-col-2"></td>\
                                                    <td class="advanced-search-col-3"></td>\
                                                    <td class="advanced-search-col-4"></td>\
                                                    <td class="advanced-search-col-5"></td>\
                                                  </tr>\
                                                </tbody>\
                                              </table>');
    
    this.addOperatorsSelector(jQuery('.advanced-search-filter-row').find('.advanced-search-col-1').last())

    /* Create the Score Boosting section */
    jQuery('#score-boosting-section > p').append('<table id="score-boosting-table">\
                                                <tbody>\
                                                </tbody>\
                                              </table>');

    /* Create the attributes phrase boosting section */
    jQuery('#phrase-boosting-section > p').append('<table id="phrase-boosting-table">\
                                                <tbody>\
                                                  <tr>\
                                                    <td style="padding-right: 20px;">\
                                                      Phrase terms distance:\
                                                      <input id="phrase-terms-distance" value="3" />\
                                                    </td>\
                                                  </tr>\
                                                  <tr>\
                                                    <td>&nbsp;</td>\
                                                  </tr>\
                                                </tbody>\
                                              </table>');
                                                        
    /* Create the Search Restrictions section */
    jQuery('#search-restrictions-section > p').append('<table id="search-restrictions-table">\
                                                <tbody>\
                                                </tbody>\
                                              </table>');
                                              
    /* Create the Other Options section */
    jQuery('#other-options-section > p').append('<table id="other-options-table">\
                                              <tbody>\
                                                <tr>\
                                                  <td style="padding-right: 20px;">\
                                                    Default operator:\
                                                  </td>\
                                                  <td>\
                                                    <div>\
                                                    <div>\
                                                      <button class="operator">AND</button>\
                                                      <button class="operator-selector">Select default operator</button>\
                                                    </div>\
                                                    <ul>\
                                                      <li class="op-and"><a href="#">AND</a></li>\
                                                      <li class="op-or"><a href="#">OR</a></li>\
                                                    </ul>\
                                                   </div>\
                                                  </td>\
                                                  <td style="padding-left: 20px;">\
                                                    <span id="operator-or-constrains-title">OR criterion:</span> <input id="operator-or-constrains" value="1" />\
                                                  </td>\
                                                </tr>\
                                              </tbody>\
                                            </table>');   
                                            
    jQuery('#other-options-table').find('.op-and').click(function(){
      jQuery(this).parent().prev().children(':first-child').html("AND");      
      jQuery('#operator-or-constrains').hide();
      jQuery('#operator-or-constrains-title').hide();
    });

    jQuery('#other-options-table').find('.op-or').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("OR");      
      jQuery('#operator-or-constrains').show();
      jQuery('#operator-or-constrains-title').show();
    });                                            
                                                                                       
    jQuery('#operator-or-constrains').hide();
    jQuery('#operator-or-constrains-title').hide();
        
    jQuery('#operator-or-constrains').button()
      .css({
              'font' : 'inherit',
             'color' : 'inherit',
        'text-align' : 'left',
           'outline' : 'none',
            'cursor' : 'text',
            'width'  : '50px'
      })
      .unbind()
      .addClass('adv-filter-value')
      .removeClass('ui-state-default')
      .addClass('ui-state-default-property-value');           
        
    jQuery('#phrase-terms-distance').button()
      .css({
              'font' : 'inherit',
             'color' : 'inherit',
        'text-align' : 'left',
           'outline' : 'none',
            'cursor' : 'text',
            'width'  : '50px'
      })
      .unbind()
      .addClass('adv-filter-value')
      .removeClass('ui-state-default')
      .addClass('ui-state-default-property-value');           
                   
    jQuery('#other-options-table').find('.operator')
       .button()
       .next()
       .button({
       text: false,
       icons: {
           primary: "ui-icon-triangle-1-s"
       }
    }).click(function () {
       var menu = jQuery(this).parent().next().show().position({
           my: "left top",
           at: "left bottom",
           of: this
       });
       jQuery(document).one("click", function () {
           menu.hide();
       });
       
       return false;
    })
       .parent()
       .buttonset()
       .next()
       .hide()
       .menu();   
    
    jQuery('#other-options-table').find('.operator').width('70px').height('34.5px');   
    
    jQuery('#search-profile-name').button()
      .css({
              'font' : 'inherit',
             'color' : 'inherit',
        'text-align' : 'left',
           'outline' : 'none',
            'cursor' : 'text',
            'width'  : '96%'
      })
      .unbind()
      .attr('style', function(i, style)
      {
          return style.replace(/width[^;]+;?/g, '');
      })
      .addClass('search-profile-name');    
                                                                                        

    /* initialize the UI by getting the first counts and statistics of what is in the datastore */
    this.search();    
  }
  
  this.isObjectProperty = function isObjectProperty(property)
  {
    if(property.type != undefined)
    {    
      if(property.type == 'http://www.w3.org/2002/07/owl#ObjectProperty')
      {
        return(true);
      }
      else
      {
        return(false);
      }
    }
    else
    {
      for(var i = 0; i < property.subPropertyOf.length; i++)
      {
        if(property.subPropertyOf[i] == 'http://www.w3.org/2002/07/owl#topObjectProperty') 
        {
          return(true);
        }
        
        if(property.subPropertyOf[i] == 'http://www.w3.org/2002/07/owl#topDataProperty') 
        {
          return(false);
        }
        
        /*
          If we didn't hit a topDataProperty or a topObjectProperty
          we try to traverse the tree and find the answer from a
          parent property
        */
        return(this.isObjectProperty(this.properties[property.subPropertyOf[i]]));
      }
    }
  }
  
  this.addOperatorsSelector = function addOperatorsSelector(parent)
  {
    parent.append('<div>\
                    <div>\
                      <button class="operator">&nbsp;</button>\
                      <button class="operator-selector">Select operator</button>\
                    </div>\
                    <ul>\
                      <li class="op-1"><a href="#">&nbsp;</a></li>\
                      <li class="op-2"><a href="#">AND</a></li>\
                      <li class="op-3"><a href="#">OR</a></li>\
                      <li class="op-4"><a href="#">NOT</a></li>\
                      <li class="op-5"><a href="#">(</a></li>\
                      <li class="op-6"><a href="#">)</a></li>\
                      <li class="op-7"><a href="#">AND (</a></li>\
                      <li class="op-8"><a href="#">OR (</a></li>\
                      <li class="op-9"><a href="#">NOT (</a></li>\
                      <li class="op-10"><a href="#">) AND (</a></li>\
                      <li class="op-11"><a href="#">) OR (</a></li>\
                      <li class="op-12"><a href="#">) NOT (</a></li>\
                    </ul>\
                   </div>');    
                   
    parent.find('.op-1').click(function(){
      jQuery(this).parent().prev().children(':first-child').html("&nbsp;");      
    });
                   
    parent.find('.op-2').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("AND");      
    });
                   
    parent.find('.op-3').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("OR");      
    });
                   
    parent.find('.op-4').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("NOT");      
    });
                   
    parent.find('.op-5').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("(");      
    });
                   
    parent.find('.op-6').click(function(){
      jQuery(this).parent().prev().children(':first-child').text(")");      
    });
                   
    parent.find('.op-7').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("AND (");      
    });
                   
    parent.find('.op-8').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("OR (");      
    });
                   
    parent.find('.op-9').click(function(){
      jQuery(this).parent().prev().children(':first-child').text("NOT (");      
    });
                   
    parent.find('.op-10').click(function(){
      jQuery(this).parent().prev().children(':first-child').text(") AND (");      
    });
                   
    parent.find('.op-11').click(function(){
      jQuery(this).parent().prev().children(':first-child').text(") OR (");      
    });
                   
    parent.find('.op-12').click(function(){
      jQuery(this).parent().prev().children(':first-child').text(") NOT (");      
    });
                   
    parent.find('.operator')
       .button()
       .next()
       .button({
       text: false,
       icons: {
           primary: "ui-icon-triangle-1-s"
       }
    }).click(function () {
       var menu = jQuery(this).parent().next().show().position({
           my: "left top",
           at: "left bottom",
           of: this
       });
       jQuery(document).one("click", function () {
           menu.hide();
       });
       
       return false;
    })
       .parent()
       .buttonset()
       .next()
       .hide()
       .menu();   
    
    parent.find('.operator').width('70px').height('34.5px');
  }
  
  this.addAdvancedFilter = function addAdvancedFilter()
  {
    jQuery('#advanced-search-section > p > table > tbody').append('<tr class="advanced-search-filter-row">\
                                                                <td class="advanced-search-col-1">\
                                                                </td>\
                                                                <td class="advanced-search-col-2">\
                                                                  <div class="ui-widget">\
                                                                    <select class="adv-filter-property"></select>\
                                                                  </div>\
                                                                </td>\
                                                                <td class="advanced-search-col-3" align="center">\
                                                                  <input class="adv-filter-value" />\
                                                                </td>\
                                                                <td class="advanced-search-col-4">\
                                                                  <button class="remove-button">remove filter</button>\
                                                                </td>\
                                                                <td class="advanced-search-col-5">\
                                                                  <button class="add-button">add filter</button>\
                                                                </td>\
                                                              </tr>'); 
                                                              
    /* Skin the value input */
    jQuery('#advanced-search-section').find('.adv-filter-value').last()
      .button()
      .css({
              'font' : 'inherit',
             'color' : 'inherit',
        'text-align' : 'left',
           'outline' : 'none',
            'cursor' : 'text',
            'width'  : '96%'
      })
      .unbind()
      .addClass('adv-filter-value')
      .attr('style', function(i, style)
      {
          return style.replace(/width[^;]+;?/g, '');
      })
      .removeClass('ui-state-default')
      .addClass('ui-state-default-property-value');     
      
    /* Initially hide the remove button */
    if(jQuery('#advanced-search-section').find('.remove-button').length <= 1)
    {
      jQuery('#advanced-search-section').find('.remove-button').last().hide();
    }
      
    jQuery('#advanced-search-section').find('.add-button').last().click(function(){
      jQuery(this).hide();
      
      jQuery(this).parent().prev().children(':first-child').show();
      
      self.addAdvancedFilter();
    }).button({
      icons: {
        primary: "ui-icon-circle-plus"
      },
      text: false
    });

    jQuery('#advanced-search-section').find('.remove-button').last().click(function(){      
      
      /* Only show the add button at for the last filter */
      if(jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().prev().children('.advanced-search-col-5').children(':first-child').show();
      }

      /* Hide the delete button if we are at the first filter line */
      if(jQuery(this).parent().parent().prev().prev().prev().prev().length == 0 &&
         jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().prev().children('.advanced-search-col-4').children(':first-child').hide();
      }  
      
      /* Hide the remove button of the next line if that next line is to become the first filter */
      if(jQuery(this).parent().parent().prev().prev().length == 0 &&
         jQuery(this).parent().parent().next().next().children('.advanced-search-col-4').children(':first-child').is(":visible") &&
         jQuery(this).parent().parent().next().next().children('.advanced-search-col-5').children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().next().next().children('.advanced-search-col-4').children(':first-child').hide();
      }    
      
      /* Remove this line, and the next line (the operator line) */
      jQuery(this).parent().parent().next().remove();
      jQuery(this).parent().parent().remove();
    }).button({
      icons: {
        primary: "ui-icon-circle-minus"
      },
      text: false
    });
    
    var comboProperties = this.getProperties();
    
    /* First value is empty */    
    jQuery('#advanced-search-section').find('.adv-filter-property').last().append('<option value=""></option>')
    
    for(var i = 0; i < comboProperties.length; i++)
    {
      jQuery('#advanced-search-section').find('.adv-filter-property').last().append('<option value="'+comboProperties[i].uri+'">'+comboProperties[i].prefLabel+'</option>')
    }
    
    jQuery('#advanced-search-section').find('.adv-filter-property').last().combobox({
      selected: function(event, ui) {
        /* check if the selected property is an object property, if it is, create a new value input */
        if(self.properties[ui.item.value].isObjectProperty)
        {  
          if(ui.item.value == 'dataset')
          {
           jQuery(this).parent().parent().next().find('.adv-filter-value').autocomplete({
                  delay: 750,
                  minLength: 0,
                  source: function (request, response) {
                
                    var values = [];
                    
                    for(var i = 0; i < self.datasets.length; i++)
                    {
                      values.push({ 
                        label: self.datasets[i].label,
                        value: self.datasets[i].uri
                      })
                    }                  
                    
                    response(values);                              
                },
                select: function (event, ui) {
                  
                  jQuery(this).data('uri', ui.item.value);
                  
                  ui.item.value = ui.item.label;
                  
                  jQuery(this).autocomplete('disable');
                  
                  jQuery(this).addClass('ui-input-field-uri');
                },
                open: function () {
                  jQuery(this).removeClass("ui-corner-all").addClass("ui-corner-top");
                },
                close: function () {
                  jQuery(this).removeClass("ui-corner-top").addClass("ui-corner-all");
                }
            }).blur(function(){
              jQuery(this).autocomplete('enable');
            }).focus(function() {
              jQuery(this).removeClass('ui-input-field-uri');
              jQuery(this).autocomplete("search", "");
            }).addClass('ui-input-field-uri');            
          }
          else
          {
            jQuery(this).parent().parent().next().find('.adv-filter-value').autocomplete({
                  delay: 750,
                  minLength: 0,
                  source: function (request, response) {                   
                  
                  var filteredTypes = '';
                  
                  for(var i = 0; i < self.properties[ui.item.value].allowedValue.length; i++)
                  {
                    if(self.properties[ui.item.value].allowedValue[i].type != 'http://www.w3.org/2002/07/owl#Thing')
                    {
                      filteredTypes += self.properties[ui.item.value].allowedValue[i].type+';';
                    }                  
                  }
                  
                  if(ui.item.value == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#type')
                  {
                    // Special handling if the selected property is rdf:type. 
                    // if rdf:type is used, we only want to autocomplete on the available types,
                    // and not all possible classes
                    var namedGraphs = '';
                    
                    var selectedDatasets = self.getSelectedDatasets();
                    
                    for(var i = 0; i < selectedDatasets.length; i++)
                    {
                      namedGraphs += "FROM <"+selectedDatasets[i]+"> \n";
                    }
                    
                    for(var i = 0; i < self.datasets.length; i++)
                    {
                      namedGraphs += "FROM NAMED <"+self.datasets[i].uri+"> \n";
                    }                    
                    
                    var sparqlQuery = 
                          "SELECT DISTINCT(?o) ?label \n"+
                           namedGraphs+" \n"+
                          "WHERE \n"+
                          "{ \n"+
                          "  ?s a ?o. \n"+
                          "  graph ?g { \n"+
                          "    ?o ?p ?label. \n"+
                          "    FILTER (?p IN(<http://purl.org/ontology/iron#prefLabel>, <http://purl.org/dc/terms/title>, <http://purl.org/dc/elements/1.1/title>, <http://xmlns.com/foaf/0.1/name>, <http://usefulinc.com/ns/doap#name>, <http://www.w3.org/2000/01/rdf-schema#label>, <http://www.w3.org/2004/02/skos/core#prefLabel>, <http://www.w3.org/2008/05/skos#prefLabel>, <http://www.geonames.org/ontology#name>)) \n"+
                          "    FILTER regex(str(?label), \""+jQuery(this)[0].element.val()+"\", \"i\") . \n"+
                          "  } \n"+
                          "} \n"+
                          (self.subjectAutocompleteLimit == -1 ? '' : 'OFFSET 0 LIMIT 15');
                          
                    jQuery.ajax({                    
                        url: self.drupal + "osf/proxy/",
                        type: "POST",
                        dataType: "json",
                        data: "accept="+encodeURIComponent('application/sparql-results+json')+
                              "&method=post"+
                              "&params="+encodeURIComponent("query="+encodeURIComponent(sparqlQuery))+
                              "&ws="+self.network+"sparql/",
                        success: function (data) {
                          
                          var values = [];
                          
                          for(var i = 0; i < data.results.bindings.length; i++)
                          {
                            values.push({
                              label: data.results.bindings[i].label.value,
                              value: data.results.bindings[i].o.value
                            })
                          }
                          
                          /* sort datasets by alphabetical order */    
                          values.sort(function(d1,d2){
                            if(d1.label.toLowerCase() < d2.label.toLowerCase()) return -1;
                            if(d1.label.toLowerCase() > d2.label.toLowerCase()) return 1;
                            return 0;
                          });                           
                          
                          response(values);
                        }
                    });
                  }
                  else
                  {
                    var filteredDatasets  = '';
                    
                    for(var i = 0; i < self.datasets.length; i++)
                    {
                      filteredDatasets += self.datasets[i].uri+';';
                    }                    
                    
                    jQuery.ajax({                    
                        url: self.drupal + "osf/proxy/",
                        type: "POST",
                        dataType: "json",
                        data: "accept=application/json"+
                              "&method=post"+
                              "&params="+encodeURIComponent("items=15")+
                                         encodeURIComponent("&include_aggregates=false")+                                       
                                         encodeURIComponent("&inference=false")+                                       
                                         encodeURIComponent("&sort=prefLabel+asc")+
                                         encodeURIComponent("&include_attributes_list=")+encodeURIComponent('http://purl.org/ontology/iron#prefLabel')+
                                         encodeURIComponent((jQuery(this)[0].element.val() == '' ? "&attributes=prefLabel::**" : "&attributes=prefLabel::"+urlencode(jQuery(this)[0].element.val()+"**")))+
                                         //encodeURIComponent((jQuery(this)[0].element.val() == '' ? "&attributes="+encodeURIComponent(encodeURIComponent(ui.item.value))+"::*" : "&attributes="+encodeURIComponent(ui.item.value)+"::*"+jQuery(this)[0].element.val()+"**"))+
                                         encodeURIComponent("&types=")+encodeURIComponent(filteredTypes)+
                                         encodeURIComponent("&datasets=")+encodeURIComponent(filteredDatasets)+                                       
                                         encodeURIComponent("&interface=")+encodeURIComponent(self.useInterface)+
                              "&ws="+self.network+"search/",
                        success: function (data) {
                          
                          var resultset = new Resultset(data);
                          
                          var values = [];
                          var uniqueObjects = [];
                          
                          for(var s in resultset.subjects)      
                          {
                            if(resultset.subjects.hasOwnProperty(s)) 
                            {
                              var subject = resultset.subjects[s];
                              
                              if(uniqueObjects.indexOf(subject.uri) == -1)
                              {
                                values.push({
                                  label: subject.getPrefLabel(),
                                  value: subject.uri
                                });
                                
                                uniqueObjects.push(subject.uri);
                              }                            
                            }
                          }       
                          
                          for(var uriFirstKey in values)
                          {
                            for(var uriSecondKey in values)
                            {
                              if(values[uriFirstKey]['value'] != values[uriSecondKey]['value'] &&
                                 values[uriFirstKey]['label'] == values[uriSecondKey]['label'])
                              {
                                values[uriFirstKey]['label'] += (self.getNamespacePrefix(values[uriFirstKey]['value']) == '' ? '' : ' ('+self.getNamespacePrefix(values[uriFirstKey]['value'])+')');
                                values[uriSecondKey]['label'] += (self.getNamespacePrefix(values[uriSecondKey]['value']) == '' ? '' : ' ('+self.getNamespacePrefix(values[uriSecondKey]['value'])+')');
                              }
                            }      
                          }                                                  
                          
                          response(values);
                        }
                    });
                  }
                },
                select: function (event, ui) {
                  
                  jQuery(this).data('uri', ui.item.value);
                  
                  ui.item.value = ui.item.label;
                  
                  jQuery(this).autocomplete('disable');
                  
                  jQuery(this).addClass('ui-input-field-uri');
                },
                open: function () {
                  jQuery(this).removeClass("ui-corner-all").addClass("ui-corner-top");
                },
                close: function () {
                  jQuery(this).removeClass("ui-corner-top").addClass("ui-corner-all");
                }
            }).blur(function(){
              jQuery(this).autocomplete('enable');
            }).focus(function() {
              jQuery(this).removeClass('ui-input-field-uri');
              jQuery(this).autocomplete("search", "");
            }).addClass('ui-input-field-uri');
          }
        }
        /* check if the selected property as a date as value */        
        else if(self.properties[ui.item.value].hasOwnProperty('allowedValue') && 
                self.properties[ui.item.value].allowedValue[0].type == 'http://www.w3.org/2001/XMLSchema#dateTime')
        {
          var adv_filter_value_parent = jQuery(this).parent().parent().next().find('.adv-filter-value').parent();
          
          jQuery(this).parent().parent().next().find('.adv-filter-value').remove();
          
          adv_filter_value_parent.append('\
           <table class="adv-filter-value" style="width: 100%">\
             <tr>\
               <td>\
                 From:\
               </td>\
               <td>\
                 <input class="adv-filter-value-date-from" />\
               </td>\
               <td style="padding-left: 15px;">\
                 To:\
               </td>\
               <td>\
                 <input class="adv-filter-value-date-to" />\
               </td>\
             </td>\
           </table>');
          
          adv_filter_value_parent.find('.adv-filter-value-date-from').last()
            .button()
            .css({
                    'font' : 'inherit',
                   'color' : 'inherit',
              'text-align' : 'left',
                 'outline' : 'none',
                  'cursor' : 'text',
                  'width'  : '96%'
            })
            .unbind()
            .addClass('adv-filter-value')
            .attr('style', function(i, style)
            {
                return style.replace(/width[^;]+;?/g, '');
            })
            .removeClass('ui-state-default')
            .addClass('ui-state-default-property-value')
            .addClass('ui-input-field-date')
            .datepicker({
              changeMonth: true,
              changeYear: true,
              yearRange: "1995:2013", 
              dateFormat: "yy-mm-dd",
            }); 
          
          adv_filter_value_parent.find('.adv-filter-value-date-to').last()            
            .button()
            .css({
                    'font' : 'inherit',
                   'color' : 'inherit',
              'text-align' : 'left',
                 'outline' : 'none',
                  'cursor' : 'text',
                  'width'  : '96%'
            })
            .unbind()
            .addClass('adv-filter-value')
            .attr('style', function(i, style)
            {
                return style.replace(/width[^;]+;?/g, '');
            })
            .removeClass('ui-state-default')
            .addClass('ui-state-default-property-value')
            .addClass('ui-input-field-date')
            .datepicker({
              changeMonth: true,
              changeYear: true,
              yearRange: "1995:2013", 
              dateFormat: "yy-mm-dd",
            });  
        } 
        else
        {
          var adv_filter_value_parent = jQuery(this).parent().parent().next().find('.adv-filter-value').parent();
          
          jQuery(this).parent().parent().next().find('.adv-filter-value').remove();
          
          adv_filter_value_parent.append('<input class="adv-filter-value" />');
          
          /* Skin the value input */
          jQuery('#advanced-search-section').find('.adv-filter-value').last()
            .button()
            .css({
                    'font' : 'inherit',
                   'color' : 'inherit',
              'text-align' : 'left',
                 'outline' : 'none',
                  'cursor' : 'text',
                  'width'  : '96%'
            })
            .unbind()
            .addClass('adv-filter-value')
            .attr('style', function(i, style)
            {
                return style.replace(/width[^;]+;?/g, '');
            })
            .removeClass('ui-state-default')
            .addClass('ui-state-default-property-value')
            .addClass('ui-input-field-text');                                    
        }
      }
    });
    
    jQuery('#advanced-search-section > p > table > tbody').append('<tr class="advanced-search-filter-row">\
                                                                <td class="advanced-search-col-1"></td>\
                                                                <td class="advanced-search-col-2"></td>\
                                                                <td class="advanced-search-col-3"></td>\
                                                                <td class="advanced-search-col-4"></td>\
                                                                <td class="advanced-search-col-5"></td>\
                                                              </tr>');    
                                                              
    this.addOperatorsSelector(jQuery('.advanced-search-filter-row').last().find('.advanced-search-col-1'))     
  }
  
  this.getProperties = function getProperties()
  {            
    if(this.initialProperties.length == 0)
    {
      for(var i = 0; i < this.propertiesCounts.length; i++)
      {
        if(this.properties.hasOwnProperty(this.propertiesCounts[i].uri))
        {
          var prop = this.properties[this.propertiesCounts[i].uri];
          
          prop['uri'] = this.propertiesCounts[i].uri;
          
          // Tag this property as being eligible to inferencing by adding "*" at the end of its name
          if(prop.allowedValue != undefined)
          {
            for(var ii = 0; ii < prop.allowedValue.length; ii++)
            {
              if(prop.allowedValue[ii].type == 'http://www.w3.org/2000/01/rdf-schema#Class' ||
                 prop.allowedValue[ii].type == 'http://www.w3.org/2002/07/owl#Class')
              {
                prop.prefLabel += ' [*]';
              }
            }
          }
          
          this.initialProperties.push(prop);
        }
        else
        {
          // Property is not existing in the ontology
          var prop = {};
          
          prop['isObjectProperty'] = false;
          prop['uri'] = this.propertiesCounts[i].uri;
          
          if(this.propertiesCounts[i].uri.lastIndexOf('#') != -1)
          {
            prop['prefLabel'] = this.propertiesCounts[i].uri.substring(this.propertiesCounts[i].uri.lastIndexOf('#') + 1, this.propertiesCounts[i].uri.length) + ' [+]';
          }
          else
          {
            prop['prefLabel'] = this.propertiesCounts[i].uri.substring(this.propertiesCounts[i].uri.lastIndexOf('/') + 1, this.propertiesCounts[i].uri.length) + ' [+]';          
          }
          
          // Add it to the list of available properties
          this.properties[this.propertiesCounts[i].uri] = prop;
          
          this.initialProperties.push(prop);
        }
      }
      
      /* sort properties by alphabetical order */    
      this.initialProperties.sort(function(p1,p2){
        if(p1.prefLabel.toLowerCase() < p2.prefLabel.toLowerCase()) return -1;
        if(p1.prefLabel.toLowerCase() > p2.prefLabel.toLowerCase()) return 1;
        return 0;
      });
    }    
    
    return(this.initialProperties);  
  }
  
  this.addScoreBoosting = function addScoreBoosting()
  {
    jQuery('#score-boosting-section > p > table > tbody').append('<tr class="score-boosting-row">\
                                                                <td class="score-boosting-col-1">\
                                                                  <div class="ui-widget">\
                                                                    <select class="score-boosting-property"></select>\
                                                                  </div>\
                                                                </td>\
                                                                <td class="score-boosting-col-2">\
                                                                  <input class="score-boosting-value" />\
                                                                </td>\
                                                                <td class="score-boosting-col-3" align="center">\
                                                                  <button class="remove-button">remove filter</button>\
                                                                </td>\
                                                                <td class="score-boosting-col-4">\
                                                                  <button class="add-button">add filter</button>\
                                                                </td>\
                                                                <td class="score-boosting-col-5">\
                                                                  <input class="score-booster" name="spinner" title="Boost score" value="" />\
                                                                </td>\
                                                              </tr>'); 
                                                              
    /* Skin the value input */
    jQuery('#score-boosting-section').find('.score-boosting-value').last()
      .button()
      .css({
              'font' : 'inherit',
             'color' : 'inherit',
        'text-align' : 'left',
           'outline' : 'none',
            'cursor' : 'text',
            'width'  : '96%'
      })
      .unbind()
      .addClass('score-boosting-value')
      .attr('style', function(i, style)
      {
          return style.replace(/width[^;]+;?/g, '');
      })
      .removeClass('ui-state-default')
      .addClass('ui-state-default-property-value');  
      
    /* Initialize the score booster's spinner */
    jQuery('#score-boosting-section').find('.score-booster').last().spinner({
      step: 1,
      numberFormat: "n"
    }).spinner( "value", 1 );

    /* Initially hide the remove button */
    if(jQuery('#score-boosting-section').find('.remove-button').length <= 1)
    {
      jQuery('#score-boosting-section').find('.remove-button').last().hide();
    }
      
    jQuery('#score-boosting-section').find('.add-button').last().click(function(){
      jQuery(this).hide();
      
      jQuery(this).parent().prev().children(':first-child').show();
      
      self.addScoreBoosting();
    }).button({
      icons: {
        primary: "ui-icon-circle-plus"
      },
      text: false
    });

    jQuery('#score-boosting-section').find('.remove-button').last().click(function(){      
      
      /* Only show the add button at for the last filter */
      if(jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().children('.score-boosting-col-4').children(':first-child').show();
      }

      /* Hide the delete button if we are at the first filter line */
      if(jQuery(this).parent().parent().prev().prev().length == 0 &&
         jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().children('.score-boosting-col-3').children(':first-child').hide();
      }  
      
      /* Hide the remove button of the next line if that next line is to become the first filter */
      if(jQuery(this).parent().parent().prev().length == 0 &&
         jQuery(this).parent().parent().next().children('.score-boosting-col-3').children(':first-child').is(":visible") &&
         jQuery(this).parent().parent().next().children('.score-boosting-col-4').children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().next().children('.score-boosting-col-3').children(':first-child').hide();
      }    
      
      /* Remove this line */
      jQuery(this).parent().parent().remove();
    }).button({
      icons: {
        primary: "ui-icon-circle-minus"
      },
      text: false
    });
    
    var comboProperties = this.getProperties();
    
    /* First value is empty */    
    jQuery('#score-boosting-section').find('.score-boosting-property').last().append('<option value=""></option>')
    
    for(var i = 0; i < comboProperties.length; i++)
    {
      jQuery('#score-boosting-section').find('.score-boosting-property').last().append('<option value="'+comboProperties[i].uri+'">'+comboProperties[i].prefLabel+'</option>')
    }
    
    jQuery('#score-boosting-section').find('.score-boosting-property').last().combobox({
      selected: function(event, ui) {
        /* check if the selected property is an object propert, if it is, create a new value input */
        if(self.properties[ui.item.value].isObjectProperty)
        {  
          if(ui.item.value == 'dataset')
          {
            jQuery(this).parent().parent().next().find('.score-boosting-value').autocomplete({
                  delay: 750,
                  minLength: 0,
                  source: function (request, response) {
                                            
                    var values = [];
                    
                    for(var i = 0; i < self.datasets.length; i++)
                    {
                      values.push({ 
                        label: self.datasets[i].label,
                        value: self.datasets[i].uri
                      })
                    }                  
                    
                    response(values);                   
                },
                select: function (event, ui) {
                  
                  jQuery(this).data('uri', ui.item.value);
                  
                  ui.item.value = ui.item.label;
                  
                  jQuery(this).addClass('ui-input-field-uri');                
                  
                  jQuery(this).autocomplete('disable');
                },
                open: function () {
                  jQuery(this).removeClass("ui-corner-all").addClass("ui-corner-top");
                },
                close: function () {                
                  jQuery(this).removeClass("ui-corner-top").addClass("ui-corner-all");
                }
            }).blur(function(){
                jQuery(this).autocomplete('enable');
            }).focus(function() {
              jQuery(this).removeClass('ui-input-field-uri');
              jQuery(this).autocomplete("search", "");
            }).removeClass('ui-input-field-text').addClass('ui-input-field-uri');            
          }
          else
          {
            jQuery(this).parent().parent().next().find('.score-boosting-value').autocomplete({
                  delay: 750,
                  minLength: 0,
                  source: function (request, response) {
                              
                  var filteredTypes = '';
                  
                  for(var i = 0; i < self.properties[ui.item.value].allowedValue.length; i++)
                  {
                    if(self.properties[ui.item.value].allowedValue[i].type != 'http://www.w3.org/2002/07/owl#Thing')
                    {
                      filteredTypes += self.properties[ui.item.value].allowedValue[i].type+';';
                    }                  
                  }
                  
                 if(ui.item.value == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#type')
                 {
                   // Special handling if the selected property is rdf:type. 
                   // if rdf:type is used, we only want to autocomplete on the available types,
                   // and not all possible classes
                   var namedGraphs = '';
                  
                   var selectedDatasets = self.getSelectedDatasets();
                  
                   for(var i = 0; i < selectedDatasets.length; i++)
                   {
                     namedGraphs += "FROM <"+selectedDatasets[i]+"> \n";
                   }
                  
                   for(var i = 0; i < self.datasets.length; i++)
                   {
                     namedGraphs += "FROM NAMED <"+self.datasets[i].uri+"> \n";
                   }                     
                    
                   var sparqlQuery = 
                         "SELECT DISTINCT(?o) ?label \n"+
                          namedGraphs+" \n"+
                         "WHERE \n"+
                         "{ \n"+
                         "  ?s a ?o. \n"+
                         "  graph ?g { \n"+
                         "    ?o ?p ?label. \n"+
                         "    FILTER (?p IN(<http://purl.org/ontology/iron#prefLabel>, <http://purl.org/dc/terms/title>, <http://purl.org/dc/elements/1.1/title>, <http://xmlns.com/foaf/0.1/name>, <http://usefulinc.com/ns/doap#name>, <http://www.w3.org/2000/01/rdf-schema#label>, <http://www.w3.org/2004/02/skos/core#prefLabel>, <http://www.w3.org/2008/05/skos#prefLabel>, <http://www.geonames.org/ontology#name>)) \n"+
                         "    FILTER regex(str(?label), \""+jQuery(this)[0].element.val()+"\", \"i\") . \n"+
                         "  } \n"+
                         "} \n"+
                         (self.subjectAutocompleteLimit == -1 ? '' : 'OFFSET 0 LIMIT 15');
                         
                   jQuery.ajax({                    
                       url: self.drupal + "osf/proxy/",
                       type: "POST",
                       dataType: "json",
                       data: "accept="+encodeURIComponent('application/sparql-results+json')+
                             "&method=post"+
                             "&params="+encodeURIComponent("query="+encodeURIComponent(sparqlQuery))+
                             "&ws="+self.network+"sparql/",
                       success: function (data) {
                         
                         var values = [];
                         
                         for(var i = 0; i < data.results.bindings.length; i++)
                         {
                           values.push({
                             label: data.results.bindings[i].label.value,
                             value: data.results.bindings[i].o.value
                           })
                         }
                         
                         /* sort datasets by alphabetical order */    
                         values.sort(function(d1,d2){
                           if(d1.label.toLowerCase() < d2.label.toLowerCase()) return -1;
                           if(d1.label.toLowerCase() > d2.label.toLowerCase()) return 1;
                           return 0;
                         });                           
                         
                         response(values);
                       }
                    });
                  }
                  else
                  {                                    
                    var filteredDatasets  = '';
                    
                    for(var i = 0; i < self.datasets.length; i++)
                    {
                      filteredDatasets += self.datasets[i].uri+';';
                    }                     
                    
                    jQuery.ajax({                    
                        url: self.drupal + "osf/proxy/",
                        type: "POST",
                        dataType: "json",
                        data: "accept=application/json"+
                              "&method=post"+
                              "&params="+encodeURIComponent("items=15")+
                                         encodeURIComponent("&include_aggregates=false")+                                       
                                         encodeURIComponent("&inference=false")+                                       
                                         encodeURIComponent("&sort=prefLabel+asc")+
                                         encodeURIComponent("&include_attributes_list=")+encodeURIComponent('http://purl.org/ontology/iron#prefLabel')+
                                         encodeURIComponent((jQuery(this)[0].element.val() == '' ? "&attributes=prefLabel::**" : "&attributes=prefLabel::"+urlencode(jQuery(this)[0].element.val()+"**")))+
                                         //encodeURIComponent((jQuery(this)[0].element.val() == '' ? "&attributes="+encodeURIComponent(encodeURIComponent(ui.item.value))+"::*" : "&attributes="+encodeURIComponent(ui.item.value)+"::*"+jQuery(this)[0].element.val()+"**"))+
                                         encodeURIComponent("&types=")+encodeURIComponent(filteredTypes)+
                                         encodeURIComponent("&datasets=")+encodeURIComponent(filteredDatasets)+                                       
                                         encodeURIComponent("&interface=")+encodeURIComponent(self.useInterface)+
                                         
                                         
                              "&ws="+self.network+"search/",
                        success: function (data) {
                          
                          var resultset = new Resultset(data);
                          
                          var values = [];
                          var uniqueObjects = [];
                          
                          for(var s in resultset.subjects)      
                          {
                            if(resultset.subjects.hasOwnProperty(s)) 
                            {
                              var subject = resultset.subjects[s];
                              
                              if(uniqueObjects.indexOf(subject.uri) == -1)
                              {
                                values.push({
                                  label: subject.getPrefLabel(),
                                  value: subject.uri
                                });
                                
                                uniqueObjects.push(subject.uri);
                              }  
                            }
                          }                              
                          
                          response(values);
                        }
                    });
                  }
                },
                select: function (event, ui) {
                  
                  jQuery(this).data('uri', ui.item.value);
                  
                  ui.item.value = ui.item.label;
                  
                  jQuery(this).addClass('ui-input-field-uri');                
                  
                  jQuery(this).autocomplete('disable');
                },
                open: function () {
                  jQuery(this).removeClass("ui-corner-all").addClass("ui-corner-top");
                },
                close: function () {                
                  jQuery(this).removeClass("ui-corner-top").addClass("ui-corner-all");
                }
            }).blur(function(){
                jQuery(this).autocomplete('enable');
            }).focus(function() {
              jQuery(this).removeClass('ui-input-field-uri');
              jQuery(this).autocomplete("search", "");
            }).removeClass('ui-input-field-text').addClass('ui-input-field-uri');            
          }          
        }
        else
        {
          var adv_filter_value_parent = jQuery(this).parent().parent().next().find('.score-boosting-value').parent();
          
          jQuery(this).parent().parent().next().find('.score-boosting-value').remove();
          
          adv_filter_value_parent.append('<input class="score-boosting-value" />');
          
          /* Skin the value input */
          jQuery('#score-boosting-section').find('.score-boosting-value').last()
            .button()
            .css({
                    'font' : 'inherit',
                   'color' : 'inherit',
              'text-align' : 'left',
                 'outline' : 'none',
                  'cursor' : 'text',
                  'width'  : '96%'
            })
            .unbind()
            .addClass('score-boosting-value')
            .attr('style', function(i, style)
            {
                return style.replace(/width[^;]+;?/g, '');
            })
            .removeClass('ui-state-default')
            .addClass('ui-state-default-property-value')
            .addClass('ui-input-field-text');                                    
        }
      }
    }); 
  }  
  
  this.addAttributePhraseBoosting = function addAttributePhraseBoosting()
  {
    jQuery('#phrase-boosting-section > p > table > tbody').append('<tr class="search-phrase-row">\
                                                                <td class="search-phrase-col-1">\
                                                                  <div class="ui-widget">\
                                                                    <select class="search-phrase-property"></select>\
                                                                  </div>\
                                                                </td>\
                                                                <td class="search-phrase-col-2">\
                                                                </td>\
                                                                <td class="search-phrase-col-3" align="center">\
                                                                  <button class="remove-button">remove filter</button>\
                                                                </td>\
                                                                <td class="search-phrase-col-4">\
                                                                  <button class="add-button">add filter</button>\
                                                                </td>\
                                                                <td class="search-phrase-col-5">\
                                                                  <input class="score-booster" name="spinner" title="Boost score" value="" />\
                                                                </td>\
                                                              </tr>'); 
      
    /* Initialize the score booster's spinner */
    jQuery('#phrase-boosting-section').find('.score-booster').last().spinner({
      step: 1,
      numberFormat: "n"
    }).spinner( "value", 1 );

    /* Initially hide the remove button */
    if(jQuery('#phrase-boosting-section').find('.remove-button').length <= 1)
    {
      jQuery('#phrase-boosting-section').find('.remove-button').last().hide();
    }
      
    jQuery('#phrase-boosting-section').find('.add-button').last().click(function(){
      jQuery(this).hide();
      
      jQuery(this).parent().prev().children(':first-child').show();
      
      self.addAttributePhraseBoosting();
    }).button({
      icons: {
        primary: "ui-icon-circle-plus"
      },
      text: false
    });

    jQuery('#phrase-boosting-section').find('.remove-button').last().click(function(){      
      
      /* Only show the add button at for the last filter */
      if(jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().children('.search-phrase-col-4').children(':first-child').show();
      }

      /* Hide the delete button if we are at the first filter line */
      if(jQuery(this).parent().parent().prev().prev().length == 0 &&
         jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().children('.search-phrase-col-3').children(':first-child').hide();
      }  
      
      /* Hide the remove button of the next line if that next line is to become the first filter */
      if(jQuery(this).parent().parent().prev().length == 0 &&
         jQuery(this).parent().parent().next().children('.search-phrase-col-3').children(':first-child').is(":visible") &&
         jQuery(this).parent().parent().next().children('.search-phrase-col-4').children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().next().children('.search-phrase-col-3').children(':first-child').hide();
      }    
      
      /* Remove this line */
      jQuery(this).parent().parent().remove();
    }).button({
      icons: {
        primary: "ui-icon-circle-minus"
      },
      text: false
    });
    
    var comboProperties = this.getProperties();
    
    /* First value is empty */    
    jQuery('#phrase-boosting-section').find('.search-phrase-property').last().append('<option value=""></option>')
    
    for(var i = 0; i < comboProperties.length; i++)
    {
      if(comboProperties[i].uri == 'dataset')
      {
        continue;
      }
      
      jQuery('#phrase-boosting-section').find('.search-phrase-property').last().append('<option value="'+comboProperties[i].uri+'">'+comboProperties[i].prefLabel+'</option>')
    }
    
    jQuery('#phrase-boosting-section').find('.search-phrase-property').last().combobox({
      selected: function(event, ui) {
      }
    }); 
  }  

  
  this.addSearchRestriction = function addSearchRestriction()
  {
    jQuery('#search-restrictions-section > p > table > tbody').append('<tr class="search-restriction-row">\
                                                                <td class="search-restriction-col-1">\
                                                                  <div class="ui-widget">\
                                                                    <select class="search-restriction-property"></select>\
                                                                  </div>\
                                                                </td>\
                                                                <td class="search-restriction-col-2">\
                                                                </td>\
                                                                <td class="search-restriction-col-3" align="center">\
                                                                  <button class="remove-button">remove filter</button>\
                                                                </td>\
                                                                <td class="search-restriction-col-4">\
                                                                  <button class="add-button">add filter</button>\
                                                                </td>\
                                                                <td class="search-restriction-col-5">\
                                                                  <input class="score-booster" name="spinner" title="Boost score" value="" />\
                                                                </td>\
                                                              </tr>'); 
      
    /* Initialize the score booster's spinner */
    jQuery('#search-restrictions-section').find('.score-booster').last().spinner({
      step: 1,
      numberFormat: "n"
    }).spinner( "value", 1 );

    /* Initially hide the remove button */
    if(jQuery('#search-restrictions-section').find('.remove-button').length <= 1)
    {
      jQuery('#search-restrictions-section').find('.remove-button').last().hide();
    }
      
    jQuery('#search-restrictions-section').find('.add-button').last().click(function(){
      jQuery(this).hide();
      
      jQuery(this).parent().prev().children(':first-child').show();
      
      self.addSearchRestriction();
    }).button({
      icons: {
        primary: "ui-icon-circle-plus"
      },
      text: false
    });

    jQuery('#search-restrictions-section').find('.remove-button').last().click(function(){      
      
      /* Only show the add button at for the last filter */
      if(jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().children('.search-restriction-col-4').children(':first-child').show();
      }

      /* Hide the delete button if we are at the first filter line */
      if(jQuery(this).parent().parent().prev().prev().length == 0 &&
         jQuery(this).parent().next().children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().prev().children('.search-restriction-col-3').children(':first-child').hide();
      }  
      
      /* Hide the remove button of the next line if that next line is to become the first filter */
      if(jQuery(this).parent().parent().prev().length == 0 &&
         jQuery(this).parent().parent().next().children('.search-restriction-col-3').children(':first-child').is(":visible") &&
         jQuery(this).parent().parent().next().children('.search-restriction-col-4').children(':first-child').is(":visible"))
      {
        jQuery(this).parent().parent().next().children('.search-restriction-col-3').children(':first-child').hide();
      }    
      
      /* Remove this line */
      jQuery(this).parent().parent().remove();
    }).button({
      icons: {
        primary: "ui-icon-circle-minus"
      },
      text: false
    });
    
    var comboProperties = this.getProperties();
    
    /* First value is empty */    
    jQuery('#search-restrictions-section').find('.search-restriction-property').last().append('<option value=""></option>')
    
    for(var i = 0; i < comboProperties.length; i++)
    {
      if(comboProperties[i].uri == 'dataset')
      {
        continue;
      }
      
      jQuery('#search-restrictions-section').find('.search-restriction-property').last().append('<option value="'+comboProperties[i].uri+'">'+comboProperties[i].prefLabel+'</option>')
    }
    
    jQuery('#search-restrictions-section').find('.search-restriction-property').last().combobox({
      selected: function(event, ui) {
      }
    }); 
  }  
    
  this.search = function search()
  {
    var extended_query = '';
    var types_boost = '';
    var datasets_boost = '';
    var attributes_boost = '';
    var search_restrictions = '';
    var search_phrases = '';
    var defaultOperator = 'and';
    var orOperatorConstrainer = '';
    var phraseTermsDistance = 3;
    
    if(this.initialized)
    {
      var attributeValueBoostingSettings = '';
      var phraseTreatmentSettings = '';
      var valueBoostingSettings = '';      
      
      var phpCode = '// Setup the structWSF network you want to query'+"\n"+
                    '$network = "'+this.network+'";'+"\n\n"+
                    '// Create the SearchQuery object'+"\n"+
                    '$search = new SearchQuery($network);'+"\n\n";

      /* Remove any possible reported error */
      var errors = false;
      
      var phpCodeExtendedQuery = '';
      var jsonCodeExtendedQuery = {};
      
      /* Get the Advanced Search filters */
      jQuery('.controlHighlight').removeClass('controlHighlight');

      /* Close the sections if a search as been performed by clicking the search button */
      jQuery( "#accordion" ).accordion({
          active: false,
        });         
      
      /* Read the advanced filters */
      for(var i = 1; i <= jQuery('.advanced-search-filter-row').length; i++)
      {
        if(i % 2 != 0)
        {
          /* This is an operator row */
          
          /* Get the operator to use in the extended query */
          
          var operatorText = jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.operator').text();
          
          if(operatorText.indexOf('AND') == -1 &&
             operatorText.indexOf('OR') == -1 &&
             operatorText.indexOf('NOT') == -1 &&
             operatorText.indexOf('(') == -1 &&
             operatorText.indexOf(')') == -1 && 
             i != 1 &&                                     // Skip the first one
             i != jQuery('.advanced-search-filter-row').length) // Skip the last one
          {
            jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.operator').addClass('controlHighlight');
            errors = true;
          }
          else
          {        
            extended_query += ' '+operatorText.replace(/^\s+|\s+$/g,"")+' ';
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == '(')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->startGrouping()'+"\n";
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == ')')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->endGrouping()'+"\n";
              jsonCodeExtendedQuery["endGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == 'AND')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->and_()'+"\n";
              jsonCodeExtendedQuery["and_"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == 'OR')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->or_()'+"\n";
              jsonCodeExtendedQuery["or_"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == 'NOT')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->not_()'+"\n";
              jsonCodeExtendedQuery["not_"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == 'AND (')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->and_()'+"\n"+
                                      '                       ->startGrouping()'+"\n";
              jsonCodeExtendedQuery["and_"] = [];
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == 'OR (')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->or_()'+"\n"+
                                      '                       ->startGrouping()'+"\n";
              jsonCodeExtendedQuery["or_"] = [];
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == 'NOT (')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->not_()'+"\n"+
                                      '                       ->startGrouping()'+"\n";
              jsonCodeExtendedQuery["not_"] = [];
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == ') AND (')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->endGrouping_()'+"\n"+
                                      '                       ->and_()'+"\n"+
                                      '                       ->startGrouping()'+"\n";
              jsonCodeExtendedQuery["endGrouping"] = [];
              jsonCodeExtendedQuery["and_"] = [];
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == ') OR (')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->endGrouping_()'+"\n"+
                                      '                       ->or_()'+"\n"+
                                      '                       ->startGrouping()'+"\n";
              jsonCodeExtendedQuery["endGrouping"] = [];
              jsonCodeExtendedQuery["or_"] = [];
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
            
            if(operatorText.replace(/^\s+|\s+$/g,"") == ') NOT (')
            {
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->endGrouping_()'+"\n"+
                                      '                       ->not_()'+"\n"+
                                      '                       ->startGrouping()'+"\n";
              jsonCodeExtendedQuery["endGrouping"] = [];
              jsonCodeExtendedQuery["not_"] = [];
              jsonCodeExtendedQuery["startGrouping"] = [];
            }
          }
        }
        else
        {
          /* This is a non-operator row */
          var selectedProperty = jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-property').val();
          
          /* Check if the value is a URI or a literal value */
          if(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri') != undefined)
          {
            /* This is a URI value */
            if(selectedProperty == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#type')
            {
              if(self.inferenceEnabled)
              {
                extended_query += 'inferred_type:"' + encodeURIComponent(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')) + '" ';
                
                phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->typeFilter("'+jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')+'", TRUE)'+"\n";
                jsonCodeExtendedQuery["typeFilter"] = [jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri'), true];
              }
              else
              {
                extended_query += 'type:"' + encodeURIComponent(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')) + '" ';
                
                phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->typeFilter("'+jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')+'", FALSE)'+"\n";
                jsonCodeExtendedQuery["typeFilter"] = [jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri'), false];
              }
            }
            else if(selectedProperty == 'dataset')
            {
              extended_query += selectedProperty + ':"' + jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri') + '" ';
                
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->datasetFilter("'+jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')+'")'+"\n";
              jsonCodeExtendedQuery["typedatasetFilterFilter"] = [jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')];
            }
            else
            {
              // Expend query here
              if(this.inferenceEnabled)
              {
                var subClasses = this.getSubClasses(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri'), 3, []);

                if(subClasses.length > 0)
                {
                  var empty = (phpCodeExtendedQuery == '' ? true : false);
                  phpCodeExtendedQuery += '                       // Inference is enabled, this means that the following filters'+"\n";
                  phpCodeExtendedQuery += '                       // have been inferred from the ontologies, and forward chained'+"\n";
                  phpCodeExtendedQuery += '                       // into this query'+"\n";
                  
                  extended_query += '(';
                  phpCodeExtendedQuery += (empty ? '$extendedFiltersBuilder' : '                       ')+'->startGrouping()'+"\n";
                  jsonCodeExtendedQuery["startGrouping"] = [];
                }
                
                extended_query += encodeURIComponent(selectedProperty) + '[uri]:"' + encodeURIComponent(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')) + '" ';
                
                phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->attributeValueFilter("'+selectedProperty+'", "'+jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')+'", TRUE)'+"\n";
                jsonCodeExtendedQuery["attributeValueFilter"] = [selectedProperty, jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri'), true];
                  
                for(var sc = 0; sc < subClasses.length; sc++)
                {
                  extended_query += ' OR ' + encodeURIComponent(selectedProperty) + '[uri]:"' + encodeURIComponent(subClasses[sc]) + '" ';
                  
                  phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->or_()->attributeValueFilter("'+selectedProperty+'", "'+subClasses[sc]+'", TRUE)'+"\n";
                  jsonCodeExtendedQuery["or_"] = [];
                  jsonCodeExtendedQuery["attributeValueFilter"] = [selectedProperty, subClasses[sc], true];
                }  
                  
                if(subClasses.length > 0)
                {
                  extended_query += ')';
                  phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->endGrouping()'+"\n";
                  jsonCodeExtendedQuery["endGrouping"] = [];
                }
              }
              else
              {
                extended_query += encodeURIComponent(selectedProperty) + '[uri]:"' + encodeURIComponent(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')) + '" ';
                
                phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->attributeValueFilter("'+selectedProperty+'", "'+jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri')+'", TRUE)'+"\n";
                jsonCodeExtendedQuery["attributeValueFilter"] = [selectedProperty, jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').data('uri'), true];
              }
            }
          }
          else
          {
            /* This is a Literal value */
            
            /* Check if it is date-time literal value */
            // ui-input-field-date
            if(selectedProperty != '' && jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.ui-input-field-date').length > 0)
            {
              var dateFrom = jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value-date-from').val();
              var dateTo = jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value-date-to').val();
              
              extended_query += encodeURIComponent(selectedProperty) + ':' + encodeURIComponent('['+(dateFrom == '' ? '*' : dateFrom+'T00:00:00Z')+' TO '+(dateTo == '' ? '*' : dateTo+'T00:00:00Z')+']') + ' ';
              
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->attributeValueFilter("'+selectedProperty+'", "['+(dateFrom == '' ? '*' : dateFrom+'T00:00:00Z')+' TO '+(dateTo == '' ? '*' : dateTo+'T00:00:00Z')+']", FALSE)'+"\n";
              jsonCodeExtendedQuery["attributeValueFilter"] = [selectedProperty, '['+(dateFrom == '' ? '*' : dateFrom+'T00:00:00Z')+' TO '+(dateTo == '' ? '*' : dateTo+'T00:00:00Z')+']', false];
            }
            else if(selectedProperty != '' && jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').val() != '')
            {
              extended_query += encodeURIComponent(selectedProperty) + ':' + encodeURIComponent(jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').val()) + ' ';
              
              phpCodeExtendedQuery += (phpCodeExtendedQuery == '' ? '$extendedFiltersBuilder' : '                       ')+'->attributeValueFilter("'+selectedProperty+'", "'+jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').val()+'", FALSE)'+"\n";
              jsonCodeExtendedQuery["attributeValueFilter"] = [selectedProperty, jQuery('.advanced-search-filter-row:eq('+(i-1)+')').find('.adv-filter-value').val(), false];
            }
          }
        }
      }    

      if(phpCodeExtendedQuery != '')
      {
        phpCodeExtendedQuery = '// Create the extended filter builder object'+"\n"+
                               '$extendedFiltersBuilder = new ExtendedFiltersBuilder();'+"\n\n"+phpCodeExtendedQuery;

        phpCode += phpCodeExtendedQuery.replace(/[\r\n]+$/g, "")+";\n\n";  
      }             
                    
      phpCode += '// Create the search query object used to generate the search query, and send it to the endpoint'+"\n";
      phpCode += '$search->includeAggregates()'+"\n";
      phpCode += '       ->includeScores()'+"\n";
      phpCode += '       ->mime("resultset")'+"\n";
      phpCode += '       ->query("'+jQuery('#search-input').val()+'")'+"\n";
      phpCode += '       ->page('+((this.currentPage - 1) * 5)+')'+"\n";
      phpCode += '       ->items('+(this.initialized ? jQuery('#nb-record-per-page-combo').find('option:selected').text() : '0')+')'+"\n";

      this.jsonQuery['includeAggregates'] = [];
      this.jsonQuery['includeScores'] = [];
      this.jsonQuery['mime'] = ['resultset'];
      this.jsonQuery['query'] = [jQuery('#search-input').val()];
      this.jsonQuery['page'] = [((this.currentPage - 1) * 5)];
      this.jsonQuery['items'] = [(this.initialized ? jQuery('#nb-record-per-page-combo').find('option:selected').text() : '0')];
      
      if(self.inferenceEnabled)
      {
        phpCode += '       ->enableInference()'+"\n";
        this.jsonQuery['enableInference'] = [];
      }
      else
      {
        phpCode += '       ->disableInference()'+"\n";
        this.jsonQuery['disableInference'] = [];
      }
      
      phpCode += '       ->sourceInterface("'+this.useInterface+'")'+"\n";
      this.jsonQuery['sourceInterface'] = [this.useInterface];

      /* Get the dataset filters */
      var selectedDatasets = this.getSelectedDatasets();
      
      var datasetFilters = '';
      
      for(var i = 0; i < selectedDatasets.length; i++)
      {
        datasetFilters += selectedDatasets[i]+';';
        
        phpCode += '       ->datasetFilter("'+selectedDatasets[i]+'")'+"\n";
        
        if(i == 0)
        {
          this.jsonQuery['datasetFilter'] = [];
        }
        
        this.jsonQuery['datasetFilter'].push([selectedDatasets[i]])
      }
      
      if(datasetFilters == '')
      {
        datasetFilters = 'all';
      }
      
      if(phpCodeExtendedQuery != '')
      {
        phpCode += '       ->extendedFilters($extendedFiltersBuilder->getExtendedFilters())'+"\n";
        this.jsonQuery['extendedFilters'] = [jsonCodeExtendedQuery];
      }
      
      /* Read the score boosting */
      for(var i = 1; i <= jQuery('.score-boosting-row').length; i++)
      {
        var selectedProperty = jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-property').val();
        var scoreBoost = jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-booster').val();
        
        if(scoreBoost != '')
        {
          /* Check if the value is a URI or a literal value */
          if(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri') != undefined)
          {
            /* This is a URI value */
            if(selectedProperty == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#type')
            {
              types_boost += encodeURIComponent(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')) + '^' + scoreBoost + ';';
              
              phpCode += '       ->typeBoost("'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'", "'+scoreBoost+'")'+"\n";
              
              if(!this.jsonQuery.hasOwnProperty('typeBoost'))
              {
                this.jsonQuery['typeBoost'] = [];
              }
              
              this.jsonQuery['typeBoost'].push([jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri'), scoreBoost]);
              
              valueBoostingSettings += 'type:"'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'" => '+scoreBoost+"\n";
            }
            else if(selectedProperty == 'dataset')            
            {
              datasets_boost += encodeURIComponent(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')) + '^' + scoreBoost + ';';
              
              phpCode += '       ->datasetBoost("'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'", "'+scoreBoost+'")'+"\n";

              if(!this.jsonQuery.hasOwnProperty('datasetBoost'))
              {
                this.jsonQuery['datasetBoost'] = [];
              }
              
              this.jsonQuery['datasetBoost'].push([jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri'), scoreBoost]);              
              
              valueBoostingSettings += 'dataset:"'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'" => '+scoreBoost+"\n";
            }
            else
            {
              // Expend query here
              if(this.inferenceEnabled)
              {
                var subClasses = this.getSubClasses(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri'), 3, []);

                if(subClasses.length > 0)
                {
                  phpCode += '       // Inference is enabled, this means that the following filters'+"\n";
                  phpCode += '       // have been inferred from the ontologies, and forward chained'+"\n";
                  phpCode += '       // into this query'+"\n";
                }

                attributes_boost += encodeURIComponent(selectedProperty) + '[uri]::' + encodeURIComponent(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')) + '^' + scoreBoost + ';';
                
                phpCode += '       ->attributeValueBoost("'+selectedProperty+'", "'+scoreBoost+'", "'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'", TRUE)'+"\n";

                if(!this.jsonQuery.hasOwnProperty('attributeValueBoost'))
                {
                  this.jsonQuery['attributeValueBoost'] = [];
                }
                
                this.jsonQuery['attributeValueBoost'].push([selectedProperty, scoreBoost, jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri'), true]);
                
                
                for(var sc = 0; sc < subClasses.length; sc++)
                {
                  attributes_boost += encodeURIComponent(selectedProperty) + '[uri]::' + encodeURIComponent(subClasses[sc]) + '^' + scoreBoost + ';';
                  
                  phpCode += '       ->attributeValueBoost("'+selectedProperty+'", "'+scoreBoost+'", "'+subClasses[sc]+'", TRUE)'+"\n";
                  
                  if(!this.jsonQuery.hasOwnProperty('attributeValueBoost'))
                  {
                    this.jsonQuery['attributeValueBoost'] = [];
                  }
                  
                  this.jsonQuery['attributeValueBoost'].push([selectedProperty, scoreBoost, subClasses[sc], true]);                  
                }  
              }
              else
              {
                attributes_boost += encodeURIComponent(selectedProperty) + '[uri]::' + encodeURIComponent(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')) + '^' + scoreBoost + ';';
                
                phpCode += '       ->attributeValueBoost("'+selectedProperty+'", "'+scoreBoost+'", "'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'", TRUE)'+"\n";
                
                if(!this.jsonQuery.hasOwnProperty('attributeValueBoost'))
                {
                  this.jsonQuery['attributeValueBoost'] = [];
                }
                
                this.jsonQuery['attributeValueBoost'].push([selectedProperty, scoreBoost, jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri'), true]);
              }
              
              valueBoostingSettings += selectedProperty+':"'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').data('uri')+'" => '+scoreBoost+"\n";                            
            }
          }
          else
          {
            /* This is a Literal value */
            if(selectedProperty != '' && jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').val() != '')
            {
              attributes_boost += encodeURIComponent(selectedProperty) + '::' + encodeURIComponent(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').val()) + '^' + scoreBoost + ';';
              
              phpCode += '       ->attributeValueBoost("'+selectedProperty+'", "'+scoreBoost+'", "'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').val()+'", FALSE)'+"\n";
              
              if(!this.jsonQuery.hasOwnProperty('attributeValueBoost'))
              {
                this.jsonQuery['attributeValueBoost'] = [];
              }
              
              this.jsonQuery['attributeValueBoost'].push([selectedProperty, scoreBoost, jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').val(), false]);
            }
            else
            {
              if(selectedProperty != '')
              {
                /* The user wants to boost the property, but not for a particular value */
                attributes_boost += encodeURIComponent(selectedProperty) + '^' + scoreBoost + ';';
                
                phpCode += '       ->attributeValueBoost("'+selectedProperty+'", "'+scoreBoost+'", "", FALSE)'+"\n";
                
                if(!this.jsonQuery.hasOwnProperty('attributeValueBoost'))
                {
                  this.jsonQuery['attributeValueBoost'] = [];
                }
                
                this.jsonQuery['attributeValueBoost'].push([selectedProperty, scoreBoost, '', FALSE]);
              }
            }

            if(jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').val() != '')
            {
              valueBoostingSettings += selectedProperty+':"'+jQuery('.score-boosting-row:eq('+(i-1)+')').find('.score-boosting-value').val()+'" => '+scoreBoost+"\n";                            
            }
          }
        }
      }
      
      /* Read the search restrictions */
      for(var i = 1; i <= jQuery('.search-restriction-row').length; i++)
      {
        var selectedProperty = jQuery('.search-restriction-row:eq('+(i-1)+')').find('.search-restriction-property').val();
        var scoreBoost = jQuery('.search-restriction-row:eq('+(i-1)+')').find('.score-booster').val();
        
        if(scoreBoost != '' && selectedProperty != '')
        {
          /* Check if the value is a URI or a literal value */
          search_restrictions += encodeURIComponent(selectedProperty) + '^' + scoreBoost + ';';
          
          phpCode += '       ->searchRestriction("'+selectedProperty+'", "'+scoreBoost+'")'+"\n";         

          if(!this.jsonQuery.hasOwnProperty('searchRestriction'))
          {
            this.jsonQuery['searchRestriction'] = [];
          }
          
          this.jsonQuery['searchRestriction'].push([selectedProperty, scoreBoost]);

          
          attributeValueBoostingSettings += selectedProperty+" => "+scoreBoost+"\n";
        }
      } 
      
      /* Read the phrase treatment */
      for(var i = 1; i <= jQuery('.search-phrase-row').length; i++)
      {
        var selectedProperty = jQuery('.search-phrase-row:eq('+(i-1)+')').find('.search-phrase-property').val();
        var scoreBoost = jQuery('.search-phrase-row:eq('+(i-1)+')').find('.score-booster').val();
        
        if(scoreBoost != '' && selectedProperty != '')
        {
          /* Check if the value is a URI or a literal value */
          search_phrases += encodeURIComponent(selectedProperty) + '^' + scoreBoost + ';';
          
          phpCode += '       ->attributePhraseBoost("'+selectedProperty+'", "'+scoreBoost+'")'+"\n";         
          
          if(!this.jsonQuery.hasOwnProperty('attributePhraseBoost'))
          {
            this.jsonQuery['attributePhraseBoost'] = [];
          }
          
          this.jsonQuery['attributePhraseBoost'].push([selectedProperty, scoreBoost]);
          
          phraseTreatmentSettings += selectedProperty+" => "+scoreBoost+"\n";          
        }
      }     
      
      /* Check the phrase terms distance */ 
      if(search_phrases != '')
      {
        phraseTermsDistance = jQuery('#phrase-terms-distance').val();
        phpCode += '       ->phraseBoostDistance("'+phraseTermsDistance+'")'+"\n";         
        this.jsonQuery['phraseBoostDistance'] = [phraseTermsDistance];        
      }
      
      /* Check for the default query operator */
      defaultOperator = jQuery('#other-options-table').find('.operator').text();
      
      if(defaultOperator == 'OR')
      {
        orOperatorConstrainer = jQuery('#operator-or-constrains').val(); 
        phpCode += '       ->defaultOperatorOR("'+orOperatorConstrainer+'")'+"\n";         
        this.jsonQuery['defaultOperatorOR'] = [orOperatorConstrainer];                
      }
      else
      {
        phpCode += '       ->defaultOperatorAND()'+"\n";         
        this.jsonQuery['defaultOperatorAND'] = [];        
      }
      
      if(errors)
      {
        jQuery( "#accordion" ).accordion({
            active: 0,
          });  
          
        return;    
      }

      phpCode += '       ->send();'+"\n\n";

      phpCode += '$resultset = $search->getResultset()'+"\n";

      this.structWSFPHPAPIQuery = phpCode;
      
      /* Generate PHP code that reflect the search query that as been built */
      jQuery('#phpCode').text(phpCode.replace(/[\r\n]+$/g, "")+";");
      
      /* Populate settings */
      jQuery('#attributesRestrictionBoostingSettings').text(attributeValueBoostingSettings.replace(/[\r\n]+$/g, ""));
      jQuery('#phraseTreatmentSettings').text(phraseTreatmentSettings.replace(/[\r\n]+$/g, ""));
      jQuery('#valueBoostingSettings').text(valueBoostingSettings.replace(/[\r\n]+$/g, ""));
      
      prettyPrint();      
      
    }
    else
    {
      datasetFilters = 'all';      
    }
    
    if(!this.initialized)
    {
      jQuery('#datasets').hide();
      
      jQuery("#datasets-progress").progressbar({
        value: false
      });      
    }
    else
    {
      jQuery('#results').empty();      
      jQuery('#paginator-canvas').hide();
      
      jQuery("#results-progress").progressbar({
        value: false
      }).show();          
    }
    /*
    this.jsonQuery.interface = this.useInterface;
    this.jsonQuery.items = (this.initialized ? jQuery('#nb-record-per-page-combo').find('option:selected').text() : '0');
    this.jsonQuery.include_aggregates = 'true';
    this.jsonQuery.page = ((this.currentPage - 1) * 5)
    this.jsonQuery.include_scores = 'true';
    this.jsonQuery.inference = (self.inferenceEnabled ? 'on' : 'off');
    this.jsonQuery.datasets = datasetFilters;
    this.jsonQuery.query = jQuery('#search-input').val();
    this.jsonQuery.default_operator = defaultOperator+(orOperatorConstrainer != '' ? '::'+orOperatorConstrainer : '');

    if(extended_query != '') {
      this.jsonQuery.extended_filters = extended_query.replace(/^\s+|\s+$/g,"");
    }
    if(attributes_boost != '') {
      this.jsonQuery.attributes_boost = attributes_boost.replace(/;$/,'');
    }
    if(types_boost != '') {
      this.jsonQuery.types_boost = types_boost.replace(/;$/,'');
    }
    if(datasets_boost != '') {
      this.jsonQuery.datasets_boost = datasets_boost.replace(/;$/,'')
    }
    if(search_restrictions != '') {
      this.jsonQuery.search_restrictions = search_restrictions;
    }
    if(search_phrases != '') {
      this.jsonQuery.attributes_phrase_boost = search_phrases;
      this.jsonQuery.phrase_boost_distance = phraseTermsDistance;
    }*/
    
    /* Generate JSON query code that reflect the search query that as been built */
    jQuery('#jsonQuery').text(JSON.stringify(this.jsonQuery, null, 2));
    
        
    jQuery.ajax({
      type: "POST",
      url: this.drupal + "osf/proxy/",
      data: "accept=application/json"+
            "&method=post"+
            "&params="+encodeURIComponent("interface="+this.useInterface)+encodeURIComponent("&items=")+(this.initialized ? jQuery('#nb-record-per-page-combo').find('option:selected').text() : '0')+
                       (extended_query != '' ? encodeURIComponent("&extended_filters=" + encodeURIComponent(extended_query.replace(/^\s+|\s+$/g,""))) : '') +
                       (attributes_boost != '' ? encodeURIComponent("&attributes_boost=" + attributes_boost.replace(/;$/,'')) : '')+
                       (types_boost != '' ? encodeURIComponent("&types_boost=" + types_boost.replace(/;$/,'')) : '')+
                       (datasets_boost != '' ? encodeURIComponent("&datasets_boost=" + datasets_boost.replace(/;$/,'')) : '')+
                       (search_restrictions != '' ? encodeURIComponent("&search_restrictions=" + search_restrictions) : '')+
                       (search_phrases != '' ? encodeURIComponent("&attributes_phrase_boost=" + search_phrases + "&phrase_boost_distance=" + phraseTermsDistance) : '')+
                       encodeURIComponent("&include_aggregates=true")+
                       encodeURIComponent("&page=")+((this.currentPage - 1) * 5)+
                       encodeURIComponent("&include_scores=true")+
                       encodeURIComponent("&inference=")+(self.inferenceEnabled ? 'on' : 'off')+
                       encodeURIComponent("&datasets=")+encodeURIComponent(datasetFilters)+
                       encodeURIComponent('&query='+urlencode(jQuery('#search-input').val()))+
                       encodeURIComponent('&default_operator='+defaultOperator+(orOperatorConstrainer != '' ? '::'+orOperatorConstrainer : ''))+
            "&ws="+this.network+"search/",
      dataType: "json",
      success: function(rset)
      {
        resultset = new Resultset(rset);
        
        self.classesCounts = [];
        self.datasetsCounts = [];
        self.propertiesCounts = [{uri: 'dataset', count: -1}];
        
        /* Check aggregates; calculate counts */
        for(var s in resultset.subjects)      
        {
          if(resultset.subjects.hasOwnProperty(s)) 
          {
            var subject = resultset.subjects[s];
            
            if(subject.type == 'http://purl.org/ontology/aggregate#Aggregate')
            {
              var property;
              var object;
              var count;
              
              for(var i = 0; i < subject.predicate.length; i++)
              {
                if(subject.predicate[i].hasOwnProperty('http://purl.org/ontology/aggregate#property'))
                {
                  property = subject.predicate[i]['http://purl.org/ontology/aggregate#property'].uri;
                }
                if(subject.predicate[i].hasOwnProperty('http://purl.org/ontology/aggregate#object'))
                {
                  object = subject.predicate[i]['http://purl.org/ontology/aggregate#object'].uri;
                }
                if(subject.predicate[i].hasOwnProperty('http://purl.org/ontology/aggregate#count'))
                {
                  count = subject.predicate[i]['http://purl.org/ontology/aggregate#count'];
                }
              }
              
              if(property == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#type')
              {
                self.classesCounts.push({uri: object, count: count});
              }
              else if(property == 'http://rdfs.org/ns/void#Dataset')
              {
                self.datasetsCounts.push({uri: object, count: count});
              }
              else if(property == 'http://www.w3.org/1999/02/22-rdf-syntax-ns#Property')
              {
                self.propertiesCounts.push({uri: object, count: count});
              }
            }
          }
        }    
        
        
        /* Refresh paginator*/
        
        var nbResults = 0;
        
        for(var nb = 0; nb < self.datasetsCounts.length; nb++)
        {
          nbResults += parseInt(self.datasetsCounts[nb].count);
        }
        
        if(nbResults > 0 && self.initialized)
        {
          jQuery('#paginator-canvas').show();
          jQuery('.nb-record-per-page').show();
          jQuery('#nb-record-per-page-combo').combobox({
            selected: function(event, ui) {
              self.search();
          }});
          jQuery('.nb-record-per-page').find('input').height('15px').width('33px');
          jQuery('.nb-record-per-page').find('a').height('26px');
          
          
          jQuery('#paginator').pagination({
              items: nbResults,
              currentPage: self.currentPage,
              itemsOnPage: (self.initialized ? jQuery('#nb-record-per-page-combo').find('option:selected').text() : '0'),
              cssStyle: 'compact-theme',
              onPageClick : function(pageNumber){
                self.currentPage = pageNumber;
                self.search();
              }              
          });
          
          jQuery('#results-indicator').text(nbResults + ' results');
        }
        else if(nbResults == 0 && self.initialized)
        {
          jQuery('#results').append('<div id="no-result" class="no-result ui-corner-all">\
                                  <center><em><strong>This search returned no result</strong></span></em></center>\
                                </div>');          
        }
        
        /* Order counts in ascending order */
        self.datasetsCounts.sort(function(a, b){
          return(b.count - a.count);
        });
        
        self.classesCounts.sort(function(a, b){
          return(b.count - a.count);
        });
        
        self.propertiesCounts.sort(function(a, b){
          return(b.count - a.count);
        });       

        if(!self.initialized)
        {
          self.getDatasets();     
        } 
        
        if(!self.initialized)
        {
          /* Initialize/skin the initial filtering controls */ 
          self.addAdvancedFilter();   
          
          /* Initialize/skin the initial score boosting controls */ 
          self.addScoreBoosting();  
          
          /* Initialize/skin the initial search restrictions controls */ 
          self.addSearchRestriction();            
          
          /* Initialize/skin the initial attributes phrase boosting controls */ 
          self.addAttributePhraseBoosting();            
          
          self.initialized = true;
        }
        else
        {
          /* If the QueryBuilder is initialized, then we display the results. */      
          if(resultset != false)
          {
            jQuery('#results-progress').hide();

            for(var s in resultset.subjects)      
            {
              if(resultset.subjects.hasOwnProperty(s)) 
              {
                var subject = resultset.subjects[s];
                
                if(subject.type != 'http://purl.org/ontology/aggregate#Aggregate')
                {   
                  var propertiesValues = '';
                  var score = '0';
                  var isPartOfDatasetLabel = '';
                  var isPartOfDatasetUri = '';                  
                                 
                  for(var i = 0; i < subject.predicate.length; i++)
                  {
                    for(var p in subject.predicate[i])      
                    {
                      var isObjectValue = false;
                      var valueUri = '';
                      
                      if(subject.predicate[i].hasOwnProperty(p)) 
                      {
                        var value = '';
                        if(subject.predicate[i][p].hasOwnProperty('uri'))
                        {
                          if(subject.predicate[i][p]['reify'])
                          {
                            value = subject.predicate[i][p].reify[0].value;
                          }
                          else
                          {
                            value = subject.predicate[i][p].uri;
                          }
                          
                          valueUri = subject.predicate[i][p].uri;
                          
                          isObjectValue = true;
                        }
                        else
                        {
                          if(subject.predicate[i][p].hasOwnProperty('value'))
                          {
                            value = subject.predicate[i][p].value;
                          }
                          else
                          {
                            value = subject.predicate[i][p];
                          }
                        }
                        
                        var property = p;
                        var propertyUri = property;
                        
                        if(property == 'http://purl.org/ontology/wsf#score')
                        {
                          score = value;
                        }
                        else if(property == 'http://purl.org/dc/terms/isPartOf')
                        {
                          for(var d = 0; d < self.datasets.length; d++)
                          {
                            if(value == self.datasets[d].uri)
                            {
                              isPartOfDatasetLabel = self.datasets[d].label;
                              isPartOfDatasetUri = self.datasets[d].uri;
                              break;
                            }
                          }
                        }
                        else
                        {
                          if(self.properties.hasOwnProperty(p))
                          {
                            property = self.properties[p].prefLabel;
                          }
                          
                          if(isObjectValue)
                          {
                            propertiesValues += '<li><span class="result-property-label" title="'+propertyUri+'">'+property+'</span>: <span class="result-value"><a target="_blank" href="'+self.drupal+'resources/'+encodeURIComponent(encodeURIComponent(valueUri))+'">'+value+'</a></span></li>';                      
                          }
                          else
                          {
                            propertiesValues += '<li><span class="result-property-label" title="'+propertyUri+'">'+property+'</span>: <span class="result-value">'+value.linkify()+'</span></li>';                      
                          }
                        }                    
                      }
                    }
                  }
                  
                  
                  var typeLabel = ''; 
                  var types = subject.getTypes();

                  for(var i = 0; i < types.length; i++)                  
                  {
                    if(self.classes[types[i]] != undefined)
                    {
                      typeLabel += '<span title="'+types[i]+'">'+self.classes[types[i]].prefLabel+"</span>";
                      
                      if(i != types.length - 1)
                      {
                        typeLabel += ', ';
                      }
                    }
                  }            
                  
                  jQuery('#results').append('<div id="'+jQuery.md5(subject.uri)+'" class="result ui-corner-all">\
                                          <span class="result-score"><strong>Score:</strong> '+score+'</span>\
                                          <h3 class="result-title">'+'<a target="_blank" href="'+self.drupal+'resources/'+encodeURIComponent(encodeURIComponent(subject.uri))+'">'+subject.getPrefLabel()+'</a>'+(typeLabel != '' ? ' <em>('+typeLabel+')</em>' : '') + '</h3>\
                                          <ul class="properties-list">\
                                            '+propertiesValues+'\
                                          </ul>\
                                          <em>Part of the <span title="'+isPartOfDatasetUri+'"><strong>'+isPartOfDatasetLabel+'</strong></span> dataset</em>\
                                        </div>');
                  
                  jQuery('#'+jQuery.md5(subject.uri)+'_img').data('subject-uri', subject.uri);
                }
              }
            }
          }
        }   
        
        /* Now that we have all the information about the counts, then  we display the datasets */
        self.displayDatasets();               
      },
      error: function(jqXHR, textStatus, error)
      {
        if(jqXHR.status == '403')
        {
          jQuery('#error-msg').dialog({title: 'Logging error'});
          jQuery('#error-msg > p').text('Please make sure that you are logged into Drupal in order to be able to use the QueryBuilder');
          jQuery('#error-msg').dialog('open');
        }
        else
        {
          jQuery('#error-msg').dialog({title: 'Error: ('+jqXHR.status+')'+' '+jqXHR.statusText});
          jQuery('#error-msg > p').text(jqXHR.responseText);
          jQuery('#error-msg').dialog('open');
        }
      }
    });    
  }
  
  /* Display all the datasets in the the dataset selection section */
  this.displayDatasets = function displayDatasets()
  {
    if(!this.initialized)
    {
      this.getDatasets();     
    }    
    else
    {
      jQuery("#datasets-progress").hide(); 
      
      jQuery('#datasets').empty();
      
      
      for(var i = 0; i < this.datasetsCounts.length; i++)
      {
        for(var ii = 0; ii < this.datasets.length; ii++)
        {
          if(this.datasetsCounts[i].uri == this.datasets[ii].uri)
          {
            jQuery('#datasets').append('<option '+(self.excludedDatasets.indexOf(this.datasets[ii].uri) == -1 ? 'selected="selected"' : '')+' value="'+this.datasets[ii].uri+'">'+this.datasets[ii].label+' ('+this.datasetsCounts[i].count+')'+'</option>');
            break;
          }
        }  
      }
      
      for(var i = 0; i < this.datasets.length; i++)
      {
        var found = false;
        for(var ii = 0; ii < this.datasetsCounts.length; ii++)
        {
          if(this.datasetsCounts[ii].uri == this.datasets[i].uri)
          {
            found = true;
            break;
          }
        }  
        
        if(!found)
        {
          jQuery('#datasets').append('<option '+(self.excludedDatasets.indexOf(this.datasets[i].uri) == -1 ? 'selected="selected"' : '')+' value="'+this.datasets[i].uri+'">'+this.datasets[i].label+' (0)'+'</option>');
        }
      }
      
      jQuery("#datasets").multiselect({
        minWidth: (jQuery('#search-tools').width() - 5)
      });
      
      jQuery('.ui-multiselect').children(':first-child').next().css('padding-left', '35px');
      
      jQuery("#datasets").bind("multiselectclick", function(event, ui){
        if(!ui.checked)
        {
          if(self.excludedDatasets.indexOf(ui.value) == -1)
          {
            self.excludedDatasets.push(ui.value);
          }
        }
        else
        {
          if(self.excludedDatasets.indexOf(ui.value) != -1)
          {
            self.excludedDatasets.splice(self.excludedDatasets.indexOf(ui.value), 1);
          }
        }

        jQuery.cookie('qb-excluded-datasets', JSON.stringify(self.excludedDatasets), { expires: 365, path: "/" });
      });      
      
      jQuery("#datasets").bind("multiselectcheckall", function(event, ui){
        self.excludedDatasets = [];
        
        jQuery.cookie('qb-excluded-datasets', JSON.stringify(self.excludedDatasets), { expires: 365, path: "/" });
      }); 
      
      
      jQuery("#datasets").bind("multiselectuncheckall", function(event, ui){
        self.excludedDatasets = [];
        
        for(var i = 0; i < self.datasets.length; i++)
        {
          self.excludedDatasets.push(self.datasets[i].uri);
        }

        jQuery.cookie('qb-excluded-datasets', JSON.stringify(self.excludedDatasets), { expires: 365, path: "/" });
      });    
      
      jQuery('#datasets').multiselect('refresh');
      
      jQuery('.ui-multiselect').css('height', jQuery('#search-tools').height())
    }
  }
  
  /* Get all the datasets available to that user */
  this.getDatasets = function getDatasets()
  {
    jQuery.ajax({
      type: "POST",
      url: this.drupal + "osf/proxy/",
      data: "accept=application/json"+
            "&method=get"+
            "&params="+encodeURIComponent("uri=all")+
            "&ws="+this.network+"dataset/read/",
      dataType: "json",
      success: function(rset)
      {
        resultset = new Resultset(rset);
        var datasets = [];

        for(var s in resultset.subjects)      
        {
          if(resultset.subjects.hasOwnProperty(s)) 
          {
            var subject = resultset.subjects[s];
            
            var labels = subject.getPredicateValues("http://purl.org/dc/terms/title");
            
            if(self.excludeDatasets.indexOf(subject.uri) !== -1)
            {
              continue;
            }
            
            self.datasets.push({
              uri: subject.uri,
              label: labels[0]
            });
          }
        }
        
        self.datasets.sort(function(d1,d2){
          if(d1.label.toLowerCase() < d2.label.toLowerCase()) return -1;
          if(d1.label.toLowerCase() > d2.label.toLowerCase()) return 1;
          return 0;
        });        
        
        self.displayDatasets();
      },
      error: function(jqXHR, textStatus, error)
      {
        if(jqXHR.status == '403')
        {
          jQuery('#error-msg').dialog({title: 'Logging error'});
          jQuery('#error-msg > p').text('Please make sure that you are logged into Drupal in order to be able to use the QueryBuilder');
          jQuery('#error-msg').dialog('open');
        }
        else
        {
          jQuery('#error-msg').dialog({title: 'Error: ('+jqXHR.status+')'+' '+jqXHR.statusText});
          jQuery('#error-msg > p').text(jqXHR.responseText);
          jQuery('#error-msg').dialog('open');
        }
      },
    });    
  }
  
  this.getSelectedDatasets = function getSelectedDatasets()
  {
    return(jQuery("#datasets").multiselect("getChecked").map(function(){    
        return this.value;   
    }).get());
  }
  
  this.saveSearchProfile = function saveSearchProfile()
  {  
    if(jQuery('#search-profile-name').val() == '')
    {
      jQuery('#error-msg').dialog({title: 'No Name'});
      jQuery('#error-msg > p').text('No search profile name has been defined. Please specify a name and save again.');
      jQuery('#error-msg').dialog('open');      
    }
    else
    {
      this.search();      
      
      jQuery.ajax({
        type: "POST",
        url: '/osf/querybuilder/save/search/profile/',
        data: "&php_profile="+urlencode('<?php '+this.structWSFPHPAPIQuery+'?>')+
              "&json_profile="+urlencode(JSON.stringify(this.jsonQuery, null, 2))+
              "&name="+urlencode(jQuery('#search-profile-name').val()),
        dataType: "json",
        success: function(rset)
        {
          jQuery('#error-msg').dialog({title: 'Search profile saved'});
          jQuery('#error-msg > p').text(rset.success);
          jQuery('#error-msg').dialog('open');
        },
        error: function(jqXHR, textStatus, error)
        {          
          if(jqXHR.status == '400')
          {
            var erroMsg = JSON.parse(jqXHR.responseText);
            
            jQuery('#error-msg').dialog({title: 'Error'});
            jQuery('#error-msg > p').text(erroMsg.error);
            jQuery('#error-msg').dialog('open');
          }
        },
      });        
    }
  }
  
  this.getNamespacePrefix = function getNamespacePrefix(uri)
  {
    var prefix = '';
    var baseUri = '';
    
    if(uri.lastIndexOf('#') != -1)
    {
      baseUri = uri.substring(0, uri.lastIndexOf('#') + 1);
    }
    else if(uri.lastIndexOf('/') != -1)
    {
      baseUri = uri.substring(0, uri.lastIndexOf('/') + 1);
    }
    
    if(baseUri != '')
    {
      for(var p in this.namespaces)
      {
        if(this.namespaces[p] == baseUri)
        {
          return(p);
        }
      }
    }
    
    
    if(uri.lastIndexOf('#') != -1)
    {
      prefix = uri.substring(uri.lastIndexOf('/') + 1, uri.lastIndexOf('#'));
    }
    else if(uri.lastIndexOf('/') != -1)
    {
      prefix = uri.substring(uri.lastIndexOf('/', uri.lastIndexOf('/') - 1) + 1, uri.lastIndexOf('/'));
    }
    
    return(prefix);
  }
  
  this.getSubClasses = function getSubClasses(classUri, degree, parents)
  {
    var subClasses = [];
    
    for(var cl in this.classes)
    {
      if(this.classes[cl].subTypeOf != undefined)
      {
        for(var i = 0; i < this.classes[cl].subTypeOf.length; i++)
        {
          if(this.classes[cl].subTypeOf[i] == classUri && parents.indexOf(cl) == -1)
          {
            subClasses.push(cl);
          }  
        }      
      }
    }
    
    if(degree > 1)
    {
      parents.push(classUri);
      
      var subSubClasses = [];
      
      for(var i = 0; i < subClasses.length; i++)
      {
        subSubClasses = this.getSubClasses(subClasses[i], degree - 1, parents);  
        subClasses.push.apply(subClasses, subSubClasses)
      }
    }
    
    return(jQuery.unique(subClasses));
  }
  
  /* Remove all duplicates from an array */
  jQuery.unique = function(arr){
    return jQuery.grep(arr,function(v,k){
        return jQuery.inArray(v,arr) === k;
    });
  };  
  
  /* Initialize the QueryBuilder */
  this.init();  
}

if(!String.linkify) {
    String.prototype.linkify = function() {

        // http://, https://, ftp://
        var urlPattern = /\b(?:https?|ftp):\/\/[a-z0-9-+&@#\/%?=~_|!:,.;]*[a-z0-9-+&@#\/%=~_|]/gim;

        // www. sans http:// or https://
        var pseudoUrlPattern = /(^|[^\/])(www\.[\S]+(\b|$))/gim;

        // Email addresses
        var emailAddressPattern = /\w+@[a-zA-Z_]+?(?:\.[a-zA-Z]{2,6})+/gim;

        return this
            .replace(urlPattern, '<a href="$&">$&</a>')
            .replace(pseudoUrlPattern, '$1<a href="http://$2">$2</a>')
            .replace(emailAddressPattern, '<a href="mailto:$&">$&</a>');
    };
}


/* 
  It is possible to overwrite the _renderItem function of the autocomplete prototype to
  manage specific display requirements of the autocomplete lists
*/
jQuery.ui.autocomplete.prototype._renderItem = function( ul, item ) {  
  return jQuery( '<li></li>' )
  .data( 'item.autocomplete', item )
  .append( '<a>' + item.label + '</a>' )
  .appendTo( ul );
};

function urlencode (str) {
  // http://kevin.vanzonneveld.net
  // +   original by: Philip Peterson
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +      input by: AJ
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   improved by: Brett Zamir (http://brett-zamir.me)
  // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +      input by: travc
  // +      input by: Brett Zamir (http://brett-zamir.me)
  // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   improved by: Lars Fischer
  // +      input by: Ratheous
  // +      reimplemented by: Brett Zamir (http://brett-zamir.me)
  // +   bugfixed by: Joris
  // +      reimplemented by: Brett Zamir (http://brett-zamir.me)
  // %          note 1: This reflects PHP 5.3/6.0+ behavior
  // %        note 2: Please be aware that this function expects to encode into UTF-8 encoded strings, as found on
  // %        note 2: pages served as UTF-8
  // *     example 1: urlencode('Kevin van Zonneveld!');
  // *     returns 1: 'Kevin+van+Zonneveld%21'
  // *     example 2: urlencode('http://kevin.vanzonneveld.net/');
  // *     returns 2: 'http%3A%2F%2Fkevin.vanzonneveld.net%2F'
  // *     example 3: urlencode('http://www.google.nl/search?q=php.js&ie=utf-8&oe=utf-8&aq=t&rls=com.ubuntu:en-US:unofficial&client=firefox-a');
  // *     returns 3: 'http%3A%2F%2Fwww.google.nl%2Fsearch%3Fq%3Dphp.js%26ie%3Dutf-8%26oe%3Dutf-8%26aq%3Dt%26rls%3Dcom.ubuntu%3Aen-US%3Aunofficial%26client%3Dfirefox-a'
  str = (str + '').toString();

  // Tilde should be allowed unescaped in future versions of PHP (as reflected below), but if you want to reflect current
  // PHP behavior, you would need to add ".replace(/~/g, '%7E');" to the following.
  return encodeURIComponent(str).replace(/!/g, '%21').replace(/'/g, '%27').replace(/\(/g, '%28').
  replace(/\)/g, '%29').replace(/\*/g, '%2A').replace(/%20/g, '+');
}
