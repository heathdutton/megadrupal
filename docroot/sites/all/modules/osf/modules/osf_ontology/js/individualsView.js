
var sliceSize = 250;
var previouslySelectedNamedEntity = "";

var importedFromExternalOntology = [];   

var lastSelectedPrefLabel = "";

function createIndividualsList()
{
  jQuery("#individualsList").show();
  
  jQuery("#individualsList").append('<form method="get" id="searchFilterForm">\
                                  <div>\
                                    <input type="text" value="" name="q" id="quicksearchinput" />\
                                  </div>\
                                </form>');
                                
      jQuery("#topInstancesRow").append('<img src="' + osf_ontologyModuleFullPath
        + '/imgs/ajax-loading-bar.gif" width="220" height="19" />');
                                
                                
  jQuery("#individualsList").append('<div id="individualsListAjaxLoadingContainer">\
                                  <img src="' + osf_ontologyModuleFullPath + '/imgs/ajax-loading-bar.gif" width="220" height="19" title="Getting the complete list of named individuals." />\
                                </div>');
                                        
  jQuery("#individualsList").append('<ul id="iList"></ul>');

  getNamedeEntitiesSlice(0);
  
  individualsListCreated = true;
}

function getNamedeEntitiesSlice(sliceNum)
{
    var data = jQuery.ajax({ type: "POST",
                        url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                        dataType: "json",
                        sliceNum: sliceNum,
                        "data": {
                                  ws: network.replace(/\/+$/,"") + "/ontology/read/",
                                  method: "post",
                                  accept: "application/json", 
                                  params: "function=getNamedIndividuals" +
                                          "&reasoner=" + useReasoner +
                                          "&ontology=" + escape(ontology) +
                                          "&parameters=" + escape("mode=list") + ";" +
                                                           escape("classuri=all") + ";" +
                                                           escape("direct=false") + ";" +
                                                           escape("limit=" + sliceSize) + ";" + 
                                                           escape("offset=" + (sliceNum * sliceSize)),
                        },  
                        success: function(msg)
                        {
                          var resultset = new Resultset(msg);
                          
                          if(resultset.subjects.length > 0)
                          {
                            for(var i = 0; i < resultset.subjects.length; i++)
                            {
                              var namedIndividualUri = resultset.subjects[i].uri;
                              var prefLabel = resultset.subjects[i].getPrefLabel();
                              
                              jQuery('#iList').append('<li class="namedIndividualListItem" title="'+namedIndividualUri+'" id="'+namedIndividualUri.replace(/[^a-zA-Z0-9]+/g, '')+'" onclick="selectIndividual(\''+namedIndividualUri+'\');">'+prefLabel+'</li>');
                              jQuery('#'+namedIndividualUri.replace(/[^a-zA-Z0-9]+/g, '')).data("uri", namedIndividualUri);
                              
                              // select the first named entity
                              if(i == 0 && sliceNum == 0)
                              {
                                selectIndividual(namedIndividualUri);
                              }                                                  
                            }
                            
                            sliceNum++;                    
                            getNamedeEntitiesSlice(sliceNum);  
                          }
                          else
                          {
                            jQuery("#individualsListAjaxLoadingContainer").remove();
                            jQuery('#quicksearchinput').quicksearch('ul#iList li');      

                            if(jQuery("#classesTree").data("niUri") != "")
                            {
                              selectIndividual(jQuery("#classesTree").data("niUri"));
                              jQuery("#individualsList").show();    
                            }                            
                            
                            createOrUpdateContextMenu();
                          }
                        },
                        error: function(jqXHR, textStatus, error)
                        {
                          jQuery('#messagesContainer').fadeOut("fast", function()
                          {
                            jQuery('#messagesContainer').empty();
                          });

                          var error = JSON.parse(jqXHR.responseText);

                          var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                          jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
                        }
                      });
}

function selectIndividual(uri)
{
  if(previouslySelectedNamedEntity != "")
  {
    jQuery('#' + previouslySelectedNamedEntity.replace(/[^a-zA-Z0-9]+/g, '')).removeClass("namedIndividualClicked");
  }
  
  jQuery('#' + uri.replace(/[^a-zA-Z0-9]+/g, '')).addClass("namedIndividualClicked");
  
  previouslySelectedNamedEntity = uri;
  
  jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function(){});
  
  jQuery.ajax({type: "POST",
          url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
          dataType: "json",
          "data": {
                    ws: network.replace(/\/+$/,"") + "/ontology/read/",
                    method: "post",
                    accept: "application/json", 
                    params: "function=getNamedIndividual" +
                            "&reasoner=" + useReasoner +
                            "&ontology=" + escape(ontology) +
                            "&parameters=" + escape("uri=" + uri)            
          },
          "success": function(data) {

            // Here we read the resultset, and compose the JSON object to feed to jsTree
            resultset = new Resultset(data);

            createNamedIndividualView(resultset);
          },
          "error": function(jqXHR, textStatus, error) {
            
            jQuery('#messagesContainer').fadeOut("fast", function()
            {
              jQuery('#messagesContainer').empty();
            });

            var error = JSON.parse(jqXHR.responseText);

            var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

            jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
          }
        });
}

function createNamedIndividualView(resultset)
{ 
  var uri = resultset.subjects[0].uri;

  if(uri == "")
  {
    // ask the user to enter a URI for this new entity

    //createIntermediaryUriView();

    return;
  }

  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  // All the attributes that have been processed by the default UI.
  // all the remaining ones will be displayed in the "Others" section.
  var processedAttributes = [];

  // Empty the record section.  
  jQuery("#ontologyViewColumRecord").empty();

  // Generate the record view panel.

  // Generate the Annotations section table 
  if(!isAdmin)
  {
    jQuery("#ontologyViewColumRecord").append('<div id="teaserBox">');
    
    // Add the preferred label
    var prefLabel = resultset.subjects[0].getPrefLabelTuple();

    if(prefLabel.length > 0)
    {
      var prefLabels = "";

      for(var i = 0; i < prefLabel.length; i++)
      {
        prefLabels += '<h2 class="readingModePrefLabel">' + prefLabel[i].label + '</h2>'
      }

      for(var i = 0; i < prefLabel.length; i++)
      {
        processedAttributes.push(prefLabel[i].attr);
      }      

      jQuery("#teaserBox").append(prefLabels);
    }    
    
    var altLabels = resultset.subjects[0].getAltLabelsTuple();

    if(altLabels.length > 0)
    {
      var altLabelsStrings = '<div class="readingModeAltLabels"><em>(';
      
      for(var i = 0; i < altLabels.length; i++)
      {   
        altLabelsStrings += altLabels[i].label + ", ";
      }
      
      altLabelsStrings = altLabelsStrings.substring(0, altLabelsStrings.length - 2);

      altLabelsStrings += ")</em></div>";
      
      jQuery("#teaserBox").append(altLabelsStrings);
      
      for(var i = 0; i < altLabels.length; i++)
      {
        processedAttributes.push(altLabels[i].attr);
      }        
    }
    
    var description = resultset.subjects[0].getDescriptionTuple();

    if(description.length > 0)
    {
      var descriptions = "";
      
      for(var i = 0; i < description.length; i++)
      {
        descriptions += '<div class="readingModeDescription">' + description[i].description + '</div>';
      }
      
      for(var i = 0; i < description.length; i++)
      {
        processedAttributes.push(description[i].attr);
      }        
      
      jQuery("#teaserBox").append(descriptions);
    }    
    
    jQuery("#ontologyViewColumRecord").append('<div style="padding-bottom: 20px;">&nbsp;</div>');
  }
  else
  {
    niAddSection("annotations", "Annotations");    

    // Add the preferred label
    var prefLabel = resultset.subjects[0].getPrefLabelTuple();

    if(prefLabel.length > 0)
    {
      var inputs = "";

      for(var i = 0; i < prefLabel.length; i++)
      {
        if(i == 0)
        {
          lastSelectedPrefLabel = prefLabel[i].label;
        }          
        
        inputs += '<input id="inputPrefLabel_' + i + '" class="attributeInputText" type="text" value="'
          + prefLabel[i].label + '" /><br />';

        jQuery('#inputPrefLabel_' + i).data("attr-uri", prefLabel[i].attr);
      }

      jQuery("#annotationsTable").append(
        '<tr class="annotationsRow"><td class="attributeText" valign="top">Preferred Label(s):</td><td valign="top">'
        + inputs + '</td></tr>');

      // Set the data associated with all created elements.
      for(var i = 0; i < prefLabel.length; i++)
      {
        jQuery('#inputPrefLabel_' + i).data("attr-uri", prefLabel[i].attr);
        jQuery('#inputPrefLabel_' + i).data("value-type", "literal");

        processedAttributes.push(prefLabel[i].attr);
      }
    }

    // Add altlabels
    addAttributeInput("annotationsTable", "inputAltLabel", "annotations", "Alternative Label", "http://purl.org/ontology/iron#altLabel")
    processedAttributes.push("http://purl.org/ontology/iron#altLabel");   

    // Add Description
    var description = resultset.subjects[0].getDescriptionTuple();

    var inputs = "";

    if(description.length > 0)
    {
      for(var i = 0; i < description.length; i++)
      {
        inputs += '<textarea id="inputDescription_' + i + '" class="attributeInputTextArea">' + description[i].description
          + '</textarea>';
      }
    }
    else
    {
      inputs += '<textarea id="inputDescription_0" class="attributeInputTextArea"></textarea>';
    }

    jQuery("#annotationsTable").append(
      '<tr class="annotationsRow"><td class="attributeText" valign="top">Description:</td><td valign="top">'
      + inputs + '</td></tr>');

    if(description.length <= 0)
    {
      jQuery('#inputDescription_0').data("attr-uri", "http://www.w3.org/2000/01/rdf-schema#comment");
      jQuery('#inputDescription_0').data("value-type", "literal");
    }
    else
    {
      // Set the data associated with all created elements.
      for(var i = 0; i < description.length; i++)
      {
        jQuery('#inputDescription_' + i).data("attr-uri", description[i].attr);
        jQuery('#inputDescription_' + i).data("value-type", "literal");

        processedAttributes.push(description[i].attr);
      }
    }
  }
  
  // niAddAnnotationDatatypePropertyFooterSection("annotationDatatype");
  // niAddAnnotationObjectPropertyFooterSection("annotationObject");

  // Add the Types
  niAddSection("types", "Types");
  
  niAddAttributeInputAutocomplete(resultset.subjects[0], "typesTable", "types", "types", "Types",
    "http://www.w3.org/1999/02/22-rdf-syntax-ns#type",  "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#Class");    
  
  processedAttributes.push("http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
  
  // Add the Data Properties
  niAddSection("dataProperty", "Data Properties");
  niAddDataPropertyFooterSection("dataProperty");   
  
  // Add the Object Properties
  niAddSection("objectProperty", "Object Properties");
  niAddObjectPropertyFooterSection("objectProperty"); 
  
  if(typeof (resultset.subjects[0]) == 'object' && 'predicate' in resultset.subjects[0])
  {
    var index = 0;

    for(var i = 0; i < resultset.subjects[0].predicate.length; i++)
    {
      for(var predicate in resultset.subjects[0].predicate[i])
      {
        var attrUri = resultset.unprefixize(predicate);

        if(processedAttributes.indexOf(attrUri) == -1)
        {
          var attrLabel = "";

          if(attrUri.lastIndexOf("#") != -1)
          {
            attrLabel = attrUri.substring(attrUri.lastIndexOf("#") + 1);
          }
          else if(attrUri.lastIndexOf("/") != -1)
          {
            
            attrLabel = attrUri.substring(attrUri.lastIndexOf("/") + 1);
          }
          else
          {
            attrLabel = attrUri;
          }

          var elemId = attrUri.replace(/[^a-zA-Z0-9]+/g, '');
          
          var values = resultset.subjects[0].getPredicateValues(attrUri);

          var inputs = "";

          for(var ii = 0; ii < values.length; ii++)
          {
            var value = "";
                                    
            if(typeof (values[ii]) == 'object' && 'uri' in values[ii] && processedAttributes.indexOf(attrUri) == -1)
            {                    
              niAddAttributeInputAutocomplete(resultset.subjects[0], "objectPropertyTable", elemId, "objectProperty", attrLabel,
                attrUri,  "http://purl.org/ontology/iron#prefLabel", "http://www.w3.org/2002/07/owl#Class;http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/2002/07/owl#NamedIndividual"); 
                
              processedAttributes.push(attrUri);
            }
            else if(processedAttributes.indexOf(attrUri) == -1)
            {
              niAddAttributeInput(resultset.subjects[0], "dataPropertyTable", elemId, "dataProperty", attrLabel, attrUri)
                
              processedAttributes.push(attrUri);
            }
          }
        }
      }
    }
  } 
  
  if(isAdmin)
  {
    jQuery('#ontologyViewColumRecord').append(
      '<div class="buttonsContainer"><input title="Save changes" id="saveButton" type="submit" value="Save" class="saveButton" /></div>');
      
    jQuery('#saveButton').bind("click", { resultset: resultset }, function(event){
      saveNamedIndividual(event.data.resultset);
    });
  }  
 
  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function(){});  
}

function niAddAttributeInput(subject, containerID, attributePrefix, rowPrefix, attributeLabel, targetPredicate)
{
  var attributes = [];
  
  if(subject != null)
  {
    attributes = subject.getPredicateValues(targetPredicate);
  }  
  
  var inputs = "";
  var hasValues = false;

  if(attributes.length > 0)
  {
    for(var i = 0; i <= attributes.length; i++)
    {
      if(i == attributes.length)
      {
        if(isAdmin)
        {
          inputs += '<input title="" id="' + attributePrefix + '_' + i + '" class="' + attributePrefix
            + 'InputText" type="text" value="" />';
          inputs += '<img id="' + attributePrefix + '_' + i + '_img" class="' + attributePrefix
            + 'AddImg structureAddImg" src="' + osf_ontologyModuleFullPath
            + '/imgs/bullet_add.png" title="add" /><br />';
        }

        break;
      }

      var val = "";

      if(typeof (attributes[i]) == 'object' && 'reify' in attributes[i])
      {
        val = attributes[i].reify[0].value;
      }
      else if(typeof (attributes[i]) == 'object' && 'uri' in attributes[i])
      {
        val = attributes[i].uri;
      }
      else
      {
        val = attributes[i];
      }

      if(isAdmin)
      {
        if(val.length > 250)
        {
          inputs += '<textarea title="' + attributes[i].uri + '" id="' + attributePrefix + '_' + i + '" class="'
            + attributePrefix + 'InputText">'+val+'</textarea>';
        }
        else
        {
          inputs += '<input title="' + attributes[i].uri + '" id="' + attributePrefix + '_' + i + '" class="'
            + attributePrefix + 'InputText" type="text" value="' + val + '" />';
        }
        
        inputs += '<img id="' + attributePrefix + '_' + i + '_img" class="' + attributePrefix
          + 'AddImg structureAddImg" src="' + osf_ontologyModuleFullPath
          + '/imgs/bullet_delete.png" title="remove" /><br />';
      }
      else
      {
        inputs += '<div id="' + attributePrefix + '_' + i + '" class="' + attributePrefix
                + 'InputText">' + val + '</div>'
                
        hasValues = true;
      }        
    }
  }
  else
  {
    if(isAdmin)
    {
      inputs += '<input id="' + attributePrefix + '_0" class="' + attributePrefix + 'InputText" type="text" value="" />';
      inputs += '<img id="' + attributePrefix + '_0_img" class="' + attributePrefix + 'AddImg structureAddImg" src="'
        + osf_ontologyModuleFullPath + '/imgs/bullet_add.png" title="add" /><br />';
    }
  }

  // If it is not an admin, and that there is no value for this attribute, then we don't 
  // display anything for it.
  if(!isAdmin && !hasValues)
  {
    return;
  }  
  
  jQuery("#" + containerID).append('<tr class="' + rowPrefix + 'Row"><td class="' + attributePrefix + 'Text" valign="top">'
    + attributeLabel + ':</td><td valign="top">' + inputs + '</td></tr>');

  // Set the data associated with all created elements.
  for(var i = 0; i <= attributes.length; i++)
  {
    if(i == attributes.length)
    {
      jQuery('#' + attributePrefix + '_' + i).data("value-uri", "");
      jQuery('#' + attributePrefix + '_' + i).data("attr-prefix", attributePrefix);
      jQuery('#' + attributePrefix + '_' + i + '_img').click(function()
      {
        var id = jQuery(this).attr("id").replace("_img", "");

        addInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));
      });
    }
    else
    {
      jQuery('#' + attributePrefix + '_' + i).data("value-uri", attributes[i].uri);
      jQuery('#' + attributePrefix + '_' + i).data("attr-prefix", attributePrefix);
      jQuery('#' + attributePrefix + '_' + i + '_img').click(function()
      {
        var id = jQuery(this).attr("id").replace("_img", "");

        removeInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));
      });
    }

    jQuery('#' + attributePrefix + '_' + i).data("attr-uri", targetPredicate);
    jQuery('#' + attributePrefix + '_' + i).data("value-type", "literal");
    jQuery('#' + attributePrefix + '_' + i).data("element-id", i);
  }
}

function niAddSection(prefix, title)
{
  jQuery("#ontologyViewColumRecord").append('<table id="'+prefix+'Table">');
  jQuery("#"+prefix+"Table").append(
    '<tr><td id="'+prefix+'TableHeader" colspan="2">'+title+'<img id="'+prefix+'HeaderImg" src="'
    + osf_ontologyModuleFullPath + '/imgs/arrow_up.png" title="Collapse panel" /></td></tr>');

  jQuery("#"+prefix+"TableHeader").click(function()
  {
    if(jQuery('#'+prefix+'Table tr').is(':hidden'))
    {
      jQuery("."+prefix+"Row").fadeIn("slow", function()
      {
        jQuery("#"+prefix+"HeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
        jQuery("#"+prefix+"HeaderImg").attr("title", "Collapse panel");
      });
    }
    else
    {
      jQuery("."+prefix+"Row").fadeOut("slow", function()
      {
        jQuery("#"+prefix+"HeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
        jQuery("#"+prefix+"HeaderImg").attr("title", "Extends panel");
      });
    }
  });
}

function niAddAnnotationObjectPropertyFooterSection(prefix)
{
  if(isAdmin)
  {
    jQuery("#ontologyViewColumRecord").append('<table id="'+prefix+'TableFooter"><tr>\
                                    <td>\
                                      <img id="' + prefix + 'AddNewAttributeIconImage" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table_add.png" \
                                           title="Add new object property" />\
                                      Add new annotation object property:\
                                    </td>\
                                    <td style="width: 300px;">\
                                      <input title="Add new attribute" id="' + prefix + 'AddNewAttribute"\
                                             class="' + prefix + 'AddNewAttributeInputText " \
                                             type="text" value="" />\
                                      <img id="' + prefix + 'AddNewAttributeIconWait" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table.png" \
                                           title="Add new annotation property" />\
                                    </td>\
                                  </tr></table>');
                                  
    var ac = jQuery("#"+prefix+"AddNewAttribute").autocomplete({
              serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
              minChars: 3,
              //delimiter: /(,|;)\s*/, // regex or character
              maxHeight: 400,
              width: 300,
              /*elementIndice: jQuery(this).data("element-id"),*/
              zIndex: 9999,
              iconID: prefix + 'AddNewAttributeIconWait',
              onSelect: function(value, data, me)
              {
                var attrUri = data.uri;
                var attrLabel = value.substring(0, value.search("&nbsp;&nbsp;&nbsp;"));
                var elemId = attrUri.replace(/[^a-zA-Z0-9]+/g, '');
                
                niAddAttributeInputAutocomplete(null, "annotationsTable", elemId, "objectProperty", attrLabel,
                  attrUri, "http://purl.org/ontology/iron#prefLabel", "http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/2002/07/owl#Class;http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/2002/07/owl#NamedIndividual"); 
                
                jQuery('#' + prefix + 'AddNewAttribute').val("");
                
                if(data.ontology != ontology)
                {
                  importedFromExternalOntology.push({
                            uri: data.uri,
                            type: data.type,
                            ontology: data.ontology
                          });
                }              
              },
              deferRequestBy: 300, //miliseconds
              params: {
                        datasets: (extendedToAllOntologies ? "all" : ontology),
                        attributes: "http://purl.org/ontology/iron#prefLabel",
                        page: 0,
                        items: 20,
                        include_aggregates: "false",
                        types: "http://www.w3.org/2002/07/owl#AnnotationProperty"
                      },
              noCache: true, //default is false, set to true to disable caching
            });

    searchAc.enable();  
  }                              
}

function niAddObjectPropertyFooterSection(prefix)
{
  if(isAdmin)
  {
    jQuery("#ontologyViewColumRecord").append('<table id="'+prefix+'TableFooter"><tr>\
                                    <td>\
                                      <img id="' + prefix + 'AddNewAttributeIconImage" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table_add.png" \
                                           title="Add new object property" />\
                                      Add new object property:\
                                    </td>\
                                    <td style="width: 300px;">\
                                      <input title="Add new attribute" id="' + prefix + 'AddNewAttribute"\
                                             class="' + prefix + 'AddNewAttributeInputText " \
                                             type="text" value="" />\
                                      <img id="' + prefix + 'AddNewAttributeIconWait" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table.png" \
                                           title="Add new object property" />\
                                    </td>\
                                  </tr></table>');
                                  
    var ac = jQuery("#"+prefix+"AddNewAttribute").autocomplete({
              serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
              minChars: 3,
              //delimiter: /(,|;)\s*/, // regex or character
              maxHeight: 400,
              width: 300,
              /*elementIndice: jQuery(this).data("element-id"),*/
              zIndex: 9999,
              iconID: prefix + 'AddNewAttributeIconWait',
              onSelect: function(value, data, me)
              {
                var attrUri = data.uri;
                var attrLabel = value.substring(0, value.search("&nbsp;&nbsp;&nbsp;"));
                var elemId = attrUri.replace(/[^a-zA-Z0-9]+/g, '');
                
                niAddAttributeInputAutocomplete(null, "objectPropertyTable", elemId, "objectProperty", attrLabel,
                  attrUri,  "http://purl.org/ontology/iron#prefLabel", "http://www.w3.org/2002/07/owl#Class;http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/2002/07/owl#NamedIndividual"); 
                
                jQuery('#' + prefix + 'AddNewAttribute').val("");
                
                if(data.ontology != ontology)
                {
                  importedFromExternalOntology.push({
                            uri: data.uri,
                            type: data.type,
                            ontology: data.ontology
                          });
                }              
              },
              deferRequestBy: 300, //miliseconds
              params: {
                        datasets: (extendedToAllOntologies ? "all" : ontology),
                        attributes: "http://purl.org/ontology/iron#prefLabel",
                        page: 0,
                        items: 20,
                        include_aggregates: "false",
                        types: "http://www.w3.org/2002/07/owl#ObjectProperty"
                      },
              noCache: true, //default is false, set to true to disable caching
            });

    searchAc.enable();  
  }                              
}

function niAddAnnotationDatatypePropertyFooterSection(prefix)
{
  if(isAdmin)
  {
    jQuery("#ontologyViewColumRecord").append('<table id="'+prefix+'TableFooter"><tr>\
                                    <td>\
                                      <img id="' + prefix + 'AddNewAttributeIconImage" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table_add.png" \
                                           title="Add new data property" />\
                                      Add new annotation data property:\
                                    </td>\
                                    <td style="width: 300px;">\
                                      <input title="Add new attribute" id="' + prefix + 'AddNewAttribute"\
                                             class="' + prefix + 'AddNewAttributeInputText " \
                                             type="text" value="" />\
                                      <img id="' + prefix + 'AddNewAttributeIconWait" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table.png" \
                                           title="Add new annotation data property" />\
                                    </td>\
                                  </tr></table>');
                                  
    var ac = jQuery("#"+prefix+"AddNewAttribute").autocomplete({
              serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
              minChars: 3,
              //delimiter: /(,|;)\s*/, // regex or character
              maxHeight: 400,
              width: 300,
              /*elementIndice: jQuery(this).data("element-id"),*/
              zIndex: 9999,
              iconID: prefix + 'AddNewAttributeIconWait',
              onSelect: function(value, data, me)
              {
                var attrUri = data.uri;
                var attrLabel = value.substring(0, value.search("&nbsp;&nbsp;&nbsp;"));
                var elemId = attrUri.replace(/[^a-zA-Z0-9]+/g, '');
                
                niAddAttributeInput(null, "annotationsTable", elemId, "dataProperty", attrLabel, attrUri)
                
                jQuery('#' + prefix + 'AddNewAttribute').val("");
                
                if(data.ontology != ontology)
                {
                  importedFromExternalOntology.push({
                            uri: data.uri,
                            type: data.type,
                            ontology: data.ontology
                          });
                }                            
              },
              deferRequestBy: 300, //miliseconds
              params: {
                        datasets: (extendedToAllOntologies ? "all" : ontology),
                        attributes: "http://purl.org/ontology/iron#prefLabel",
                        page: 0,
                        items: 20,
                        include_aggregates: "false",
                        types: "http://www.w3.org/2002/07/owl#AnnotationProperty"
                      },
              noCache: true, //default is false, set to true to disable caching
            });

    searchAc.enable();   
  }                             
}

function niAddDataPropertyFooterSection(prefix)
{
  if(isAdmin)
  {
    jQuery("#ontologyViewColumRecord").append('<table id="'+prefix+'TableFooter"><tr>\
                                    <td>\
                                      <img id="' + prefix + 'AddNewAttributeIconImage" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table_add.png" \
                                           title="Add new data property" />\
                                      Add new data property:\
                                    </td>\
                                    <td style="width: 300px;">\
                                      <input title="Add new attribute" id="' + prefix + 'AddNewAttribute"\
                                             class="' + prefix + 'AddNewAttributeInputText " \
                                             type="text" value="" />\
                                      <img id="' + prefix + 'AddNewAttributeIconWait" \
                                           src="' + osf_ontologyModuleFullPath + '/imgs/table.png" \
                                           title="Add new data property" />\
                                    </td>\
                                  </tr></table>');
                                  
    var ac = jQuery("#"+prefix+"AddNewAttribute").autocomplete({
              serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
              minChars: 3,
              //delimiter: /(,|;)\s*/, // regex or character
              maxHeight: 400,
              width: 300,
              /*elementIndice: jQuery(this).data("element-id"),*/
              zIndex: 9999,
              iconID: prefix + 'AddNewAttributeIconWait',
              onSelect: function(value, data, me)
              {
                var attrUri = data.uri;
                var attrLabel = value.substring(0, value.search("&nbsp;&nbsp;&nbsp;"));
                var elemId = attrUri.replace(/[^a-zA-Z0-9]+/g, '');
                
                niAddAttributeInput(null, "dataPropertyTable", elemId, "dataProperty", attrLabel, attrUri)
                
                jQuery('#' + prefix + 'AddNewAttribute').val("");
                
                if(data.ontology != ontology)
                {
                  importedFromExternalOntology.push({
                            uri: data.uri,
                            type: data.type,
                            ontology: data.ontology
                          });
                }                            
              },
              deferRequestBy: 300, //miliseconds
              params: {
                        datasets: (extendedToAllOntologies ? "all" : ontology),
                        attributes: "http://purl.org/ontology/iron#prefLabel",
                        page: 0,
                        items: 20,
                        include_aggregates: "false",
                        types: "http://www.w3.org/2002/07/owl#DatatypeProperty"
                      },
              noCache: true, //default is false, set to true to disable caching
            });

    searchAc.enable();   
  }                             
}

function niAddAttributeInputAutocomplete(subject, containerID, attributePrefix, rowPrefix, attributeLabel, targetPredicate,
                                         searchFilterAttributes, searchFilterTypes)
{
  var attributes = [];
  if(subject != null)
  {
    attributes = subject.getPredicateValues(targetPredicate);
  }

  var inputs = "";
  var hasValues = false;

  if(attributes.length > 0)
  {
    for(var i = 0; i <= attributes.length; i++)
    {
      if(i == attributes.length)
      {
        if(isAdmin)
        {
          inputs += '<input title="" id="' + attributePrefix + '_' + i + '" class="' + attributePrefix + 'InputText '
            + '" type="text" value="" />';
          inputs += '<img id="' + attributePrefix + '_' + i + '_img" class="' + attributePrefix
            + 'AddImg structureAddImg" src="' + osf_ontologyModuleFullPath
            + '/imgs/bullet_add.png" title="add" /><br />';
        }

        break;
      }

      var val = "";
      var title = "";

      if(typeof (attributes[i]) == 'object' && 'reify' in attributes[i])
      {
        val = attributes[i].reify[0].value;
      }
      else if(typeof (attributes[i]) == 'object' && 'uri' in attributes[i])
      {
        val = attributes[i].uri;
        title = attributes[i].uri;
      }
      else
      {
        val = attributes[i];
      }

      if(isAdmin)
      {
        inputs += '<input title="' + title + '" id="' + attributePrefix + '_' + i + '" class="'
          + attributePrefix + 'InputText" type="text" value="' + val + '" />';
        inputs += '<img id="' + attributePrefix + '_' + i + '_img" class="' + attributePrefix
          + 'AddImg structureAddImg" src="' + osf_ontologyModuleFullPath
          + '/imgs/bullet_delete.png" title="remove" /><br />';
      }
      else
      {
        inputs += '<div id="' + attributePrefix + '_' + i + '" class="' + attributePrefix
                + 'InputText">' + val + '</div>'
                
        hasValues = true;
      } 
    }
  }
  else
  {
    if(isAdmin)
    {
      inputs += '<input id="' + attributePrefix + '_0" class="' + attributePrefix + 'InputText '
        + '" type="text" value="" />';
      inputs += '<img id="' + attributePrefix + '_0_img" class="' + attributePrefix + 'AddImg structureAddImg" src="'
        + osf_ontologyModuleFullPath + '/imgs/bullet_add.png" title="add" /><br />';
    }
  }
  
  // If it is not an admin, and that there is no value for this attribute, then we don't 
  // display anything for it.
  if(!isAdmin && !hasValues)
  {
    return;
  }    

  jQuery("#" + containerID).append('<tr class="' + rowPrefix + 'Row"><td class="' + attributePrefix + 'Text" valign="top">'
    + attributeLabel + ':</td><td valign="top">' + inputs + '</td></tr>');

  // Set the data associated with all created elements.
  if(isAdmin)
  {
    for(var i = 0; i <= attributes.length; i++)
    {
      if(i == attributes.length)
      {
        jQuery('#' + attributePrefix + '_' + i).data("value-uri", "");
        jQuery('#' + attributePrefix + '_' + i).data("attr-prefix", attributePrefix);
        jQuery('#' + attributePrefix + '_' + i + '_img').click(function()
        {
          var id = jQuery(this).attr("id").replace("_img", "");

          niAddInputAutocomplete(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"),
            jQuery("#" + id).data("search-filter-attributes"), jQuery("#" + id).data("search-filter-types"));
        });
      }
      else
      {
        jQuery('#' + attributePrefix + '_' + i).data("value-uri", attributes[i].uri);
        jQuery('#' + attributePrefix + '_' + i).data("attr-prefix", attributePrefix);
        jQuery('#' + attributePrefix + '_' + i + '_img').click(function()
        {
          var id = jQuery(this).attr("id").replace("_img", "");

          removeInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));
        });
      }

      jQuery('#' + attributePrefix + '_' + i).data("attr-uri", targetPredicate);
      jQuery('#' + attributePrefix + '_' + i).data("value-type", "uri");
      jQuery('#' + attributePrefix + '_' + i).data("element-id", i);
      jQuery('#' + attributePrefix + '_' + i).data("search-filter-attributes", searchFilterAttributes);
      jQuery('#' + attributePrefix + '_' + i).data("search-filter-types", searchFilterTypes);

      // Now add the "input-click" behaviors to the input controls
      jQuery('#' + attributePrefix + '_' + i).focus(function()
      {
        jQuery(this).data("inputText", jQuery(this).val());
        jQuery(this).val("");

        var ac = jQuery(this).autocomplete({
                  serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                  structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
                  minChars: 3,
                  //delimiter: /(,|;)\s*/, // regex or character
                  maxHeight: 400,
                  width: 300,
                  elementIndice: jQuery(this).data("element-id"),
                  zIndex: 9999,
                  iconID: jQuery(this).attr("id") + '_img',
                  onSelect: function(value, data, me)
                  {
                    me.data("value-uri", data.uri);
                    me.attr("title", data.uri);
                    me.data("inputText", value.substring(0, value.search("&nbsp;&nbsp;&nbsp;")));

                    if(data.ontology != ontology)
                    {
                      importedFromExternalOntology.push({
                                uri: data.uri,
                                type: data.type,
                                ontology: data.ontology
                              });
                    }

                    // Add a new input on selection of an item.

                    niAddInputAutocomplete(me.data("attr-prefix"), me.data("element-id"), 
                      me.data("search-filter-attributes"), me.data("search-filter-types"));
                  },
                  deferRequestBy: 300, //miliseconds
                  params: {
                            datasets: (extendedToAllOntologies ? "all" : ontology),
                            attributes: searchFilterAttributes,
                            page: 0,
                            items: 20,
                            include_aggregates: "false",
                            types: searchFilterTypes
                          },
                  noCache: false, //default is false, set to true to disable caching
                });

        searchAc.enable();

        jQuery(this).focusout(function()
        {
          jQuery(this).val(jQuery(this).data("inputText"));
          ac = null;
        })
      })
    }
  }
}

function niAddInputAutocomplete(id, num, searchFilterAttributes, searchFilterTypes)
{
  var input = "";

  input += '<input title="" id="' + id + '_' + (num + 1) + '" class="' + id + 'InputText '
    + '" type="text" value="" />';

  input += '<img id="' + id + '_' + (num + 1) + '_img" class="' + id + 'AddImg structureAddImg" src="'
    + osf_ontologyModuleFullPath + '/imgs/bullet_add.png" title="add" /><br />';

  jQuery('#' + id + '_' + num).parent().append(input);

  jQuery('#' + id + '_' + (num + 1)).data("value-uri", "");
  jQuery('#' + id + '_' + (num + 1)).data("attr-uri", jQuery('#' + id + '_' + num).data("attr-uri"));
  jQuery('#' + id + '_' + (num + 1)).data("value-type", "uri");
  jQuery('#' + id + '_' + (num + 1)).data("element-id", (num + 1));
  jQuery('#' + id + '_' + (num + 1)).data("attr-prefix", id);
  jQuery('#' + id + '_' + (num + 1)).data("search-filter-attributes", searchFilterAttributes);
  jQuery('#' + id + '_' + (num + 1)).data("search-filter-types", searchFilterTypes);

  jQuery('#' + id + '_' + (num + 1) + '_img').click(function()
  {
    var id = jQuery(this).attr("id").replace("_img", "");
    niAddInputAutocomplete(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"),
      jQuery("#" + id).data("search-filter-attributes"), jQuery("#" + id).data("search-filter-types"));
  });

  jQuery('#' + id + '_' + num + '_img').attr("src", osf_ontologyModuleFullPath + '/imgs/bullet_delete.png');
  jQuery('#' + id + '_' + num + '_img').attr("title", 'remove');
  jQuery('#' + id + '_' + num + '_img').unbind("click");

  jQuery('#' + id + '_' + num + '_img').click(function()
  {
    var id = jQuery(this).attr("id").replace("_img", "");
    removeInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));
  });

  jQuery('#' + id + '_' + (num + 1)).focus(function()
  {
    jQuery(this).data("inputText", jQuery(this).val());
    jQuery(this).val("");

    var ac = jQuery(this).autocomplete({
              serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
              minChars: 3,
              //delimiter: /(,|;)\s*/, // regex or character
              maxHeight: 400,
              width: 300,
              elementIndice: jQuery(this).data("element-id"),
              zIndex: 9999,
              iconID: jQuery(this).attr("id") + '_img',
              onSelect: function(value, data, me)
              {
                me.data("value-uri", data.uri);
                me.attr("title", data.uri);
                me.data("inputText", value);

                if(data.ontology != ontology)
                {
                  importedFromExternalOntology.push({
                            uri: data.uri,
                            type: data.type,
                            ontology: data.ontology
                          });
                }

                // Add a new input on selection of an item.

                niAddInputAutocomplete(me.data("attr-prefix"), me.data("element-id"),
                  me.data("search-filter-attributes"), me.data("search-filter-types"));
              },
              deferRequestBy: 300, //miliseconds
              params: {
                        datasets: (extendedToAllOntologies ? "all" : ontology),
                        attributes: jQuery(this).data("search-filter-attributes"),
                        page: 0,
                        items: 20,
                        include_aggregates: "false",
                        types: jQuery(this).data("search-filter-types")
                      },
              noCache: false, //default is false, set to true to disable caching
            });

    searchAc.enable();

    jQuery(this).focusout(function()
    {
      jQuery(this).val(jQuery(this).data("inputText"));
      ac = null;
    })
  })
}

function saveNamedIndividual(resultset)
{
  jQuery("#saveButton").hide();

  if(jQuery("#saveButtonAjax").length <= 0)
  {
    jQuery("#ontologyViewColumRecord").append('<div class="buttonsContainer"><img id="saveButtonAjax" src="'
      + osf_ontologyModuleFullPath + '/imgs/ajax-loading-bar.gif" width="220" height="19" /></div>');
  }
  else
  {
    jQuery("#saveButtonAjax").show();
  }

  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  // First, let's take into account any changes that may have occured in the record's view page

  var predicates = [];

  // Get types
  var type = "http://www.w3.org/2002/07/owl#Thing";
  
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#types_" + i).length > 0 && jQuery("#types_" + i).val().length > 0)
    {
      if(i == 0)
      {
        type = jQuery("#types_" + i).data("value-uri"); 
      }
      else
      {
        var typesObj = {
                };

        typesObj[jQuery("#types_" + i).data("attr-uri")] = jQuery("#types_" + i).data("value-uri");
        predicates.push(typesObj);        
      }
    }
    else
    {
      break;
    }
  }  
    
  // Get all properties/values across all table
  jQuery('#annotationsTable input,\
     #annotationsTable textarea,\
     #dataPropertyTable input,\
     #dataPropertyTable textarea,\
     #objectPropertyTable input').each(function(index, para){
    if(jQuery(para).val() != "")
    {
      var propObj = {};

      if(jQuery(para).data("value-uri") != undefined && jQuery(para).data("value-uri") != "")
      {
        propObj[jQuery(para).data("attr-uri")] = { uri: jQuery(para).data("value-uri") };
        predicates.push(propObj);           
      }
      else
      {
        propObj[jQuery(para).data("attr-uri")] = jQuery(para).val();
        predicates.push(propObj);    
      }
    }
  });  

  rset = new Resultset({
            prefixes: resultset.prefixes,
            resultset: {
                      subject: [
                        {
                                  uri: resultset.subjects[0].uri,
                                  type: type,
                                  predicate: []
                                }
                      ]
                    }
          });

  rset.subjects[0].predicate = predicates;
   
  // Now add the classes or properties that could have been referenced that comes from external ontologies.
  // The goal here is to "import" them into the current ontology for future use
  for(i = 0; i < importedFromExternalOntology.length; i++)
  {
    var func = "";
    var typeParam = "";

    switch(importedFromExternalOntology[i].type)
    {
      case "class":
        func = "getClass";
        break;

      case "dataproperty":
      case "objectproperty":
      case "annotationproperty":
        func = "getProperty";
        break;

      case "namedindividual":
        func = "getNamedIndividual";
        break;
    }

    var entityResultset = jQuery.ajax({
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              dataType: "json",
              data: ({
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=" + func +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(importedFromExternalOntology[i].ontology) +
                                "&parameters=" + escape("uri=" + importedFromExternalOntology[i].uri)                
                      }),
              async: false
            }).responseText;

    var jsonResponseText = JSON.parse(entityResultset);

    if('level' in jsonResponseText && 'debugInformation' in jsonResponseText)
    {
      jQuery('#messagesContainer').fadeOut("fast", function()
      {
        jQuery('#messagesContainer').empty();
      });

      var errorMsg = '[' + jsonResponseText.id + '] ' + jsonResponseText.name + ': ' + jsonResponseText.description;

      jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
      continue;
    }

    entityResultset = new Resultset(jsonResponseText);

    // Remove possible superClassOf, subClassOf, equivalentClass, disjointWith relationships
    // since we don't want to create new external classes on the fly.
    entityResultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superClassOf");
    entityResultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superPropertyOf");

    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#equivalentClass");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#disjointWith");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#equivalentObjectProperties");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#equivalentDataProperties");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#disjointObjectProperties");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#disjointDataProperties");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2002/07/owl#inverseOf");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2000/01/rdf-schema#domain");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2000/01/rdf-schema#range");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2000/01/rdf-schema#subClassOf");
    entityResultset.subjects[0].removePredicateValues("http://www.w3.org/2000/01/rdf-schema#subPropertyOf");

    rset.subjects.push(entityResultset.subjects[0]);
  }
        
  importedFromExternalOntology = [];
  
  jQuery.ajax({
    type: "POST",
    url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
    dataType: "json",
    "data": {
              ws: network.replace(/\/+$/,"") + "/ontology/update/",
              method: "post",
              accept: "application/json",
              params: "ontology=" + escape(ontology) + "&function=createOrUpdateEntity&parameters="
                + escape("document=" + escape(rset.saveN3())) + ";" + escape("advancedIndexation=true")
                + "&reasoner=" + useReasoner
            },
    "success": function(data)
    {
      namedIndividualSaveSuccess();
    },
    "error": function(jqXHR, textStatus, error)
    {
      // The Ontology: Update web service does return an empty body, and not an empty JSON object.
      // This means that we have to rely on the HTTP status to know if the query is a success or 
      // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
      // string which returns a parse error.
      if(jqXHR.status == 200)
      {
        namedIndividualSaveSuccess();
      }
      else
      {
        jQuery('#messagesContainer').fadeOut("fast", function()
        {
          jQuery('#messagesContainer').empty();

          jQuery("#saveButtonAjax").hide();
          jQuery("#saveButton").show();
        });

        if(jqXHR.status == 403)
        {
          jQuery('#messagesContainer').append('<div class="errorBox">You need to have administrator permissions to modify an ontology.</div>').hide().fadeIn("fast");
        }
        else
        {
          jQuery('#messagesContainer').append('<div class="errorBox">An error occured; please record this issue to the system administrator.</div>').hide().fadeIn("fast");
        }
      }
    }
  });
}

function namedIndividualSaveSuccess()
{
  jQuery('#messagesContainer').append('<div class="successBox">Named Individual successfully saved</div>').hide().fadeIn(
    "fast");

  // If we have a new named individual, we add it to the list
  if(jQuery("#"+resultset.subjects[0].uri.replace(/[^a-zA-Z0-9]+/g, '')).length == 0)
  {
    jQuery('#iList').append('<li class="namedIndividualListItem" title="'+resultset.subjects[0].uri+'" id="'+resultset.subjects[0].uri.replace(/[^a-zA-Z0-9]+/g, '')+'" onclick="selectIndividual(\''+resultset.subjects[0].uri+'\');">'+jQuery("#inputPrefLabel_0").val()+'</li>');
    jQuery('#'+jQuery("#uriRenameInputText").val().replace(/[^a-zA-Z0-9]+/g, '')).data("uri", resultset.subjects[0].uri);
    
    createOrUpdateContextMenu();   
    
    selectIndividual(resultset.subjects[0].uri);        
  } 
  else
  { 
    // Update the prefLabel if it changed
    if(jQuery("#inputPrefLabel_0").val() != lastSelectedPrefLabel)
    {
      jQuery("#"+resultset.subjects[0].uri.replace(/[^a-zA-Z0-9]+/g, '')).text(jQuery("#inputPrefLabel_0").val());
      lastSelectedPrefLabel = jQuery("#inputPrefLabel_0").val();
    }
  }
    
  jQuery("#saveButtonAjax").hide();
  jQuery("#saveButton").show();  
}

function deleteNamedIndividual(uri)
{
  // Delete the record
  jQuery.ajax({
    type: "POST",
    url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
    dataType: "json",
    "data": {
              ws: network.replace(/\/+$/,"") + "/ontology/delete/",
              method: "post",
              accept: "application/json",
              params: "ontology=" + escape(ontology) + "&function=deleteNamedIndividual&parameters="
                + escape("uri=" + uri)
            },
    "success": function(data)
    {
      namedIndividualDeleteSuccess();
    },
    "error": function(jqXHR, textStatus, error)
    {
      // The Ontology: Update web service does return an empty body, and not an empty JSON object.
      // This means that we have to rely on the HTTP status to know if the query is a success or 
      // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
      // string which returns a parse error.
      if(jqXHR.status == 200)
      {
        namedIndividualDeleteSuccess();
      }
      else
      {      
        jQuery('#messagesContainer').fadeOut("fast", function()
        {
          jQuery('#messagesContainer').empty();
        });

        if(jqXHR.status == 403)
        {
          jQuery('#messagesContainer').append('<div class="errorBox">You need to have administrator permissions to modify an ontology.</div>').hide().fadeIn("fast");
        }
        else
        {
          jQuery('#messagesContainer').append('<div class="errorBox">An error occured; please record this issue to the system administrator.</div>').hide().fadeIn("fast");
        }
      }
    }
  });
}

function namedIndividualDeleteSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#'+uri.replace(/[^a-zA-Z0-9]+/g, '')).remove();

  jQuery('#messagesContainer').append(
    '<div class="successBox">Named Individual successfully deleted</div>').hide().fadeIn("fast");
}

function createNamedIndividualUriView(uri)
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function()
  {
  });

  // Empty the record section.
  jQuery("#ontologyViewColumRecord").empty();

  // Generate the URI Rename section table
  jQuery("#ontologyViewColumRecord").append('<table id="uriRenameTable">');
  jQuery("#uriRenameTable").append('<tr><td id="uriRenameTableHeader" colspan="2">Rename URI</td></tr>');

  jQuery(
    "#uriRenameTable").append(
    '<tr class="uriRenameRow"><td class="uriRenameText" valign="top">URI:</td><td valign="top"><input id="uriRenameInputText" class="uriRenameInputText" type="text" value="'
    + uri + '" /></td></tr>');

  jQuery('#ontologyViewColumRecord').append(
    '<input title="Rename" id="renameButton" type="submit" value="Rename" onclick="renameNamedIndividualUri(\''
    + uri + '\');" class="renameButton" />');

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
  {
  });
}

function renameNamedIndividualUri(oldUri)
{
  jQuery("#renameButton").hide();

  if(jQuery("#renameButtonAjax").length <= 0)
  {
    jQuery("#ontologyViewColumRecord").append('<div style="text-align: center; width: 100%"><img id="renameButtonAjax" src="'
      + osf_ontologyModuleFullPath + '/imgs/ajax-loading-bar.gif" width="220" height="19" /></div>');
  }
  else
  {
    jQuery("#renameButtonAjax").show();
  }

  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery.ajax({
    type: "POST",
    url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
    dataType: "json",
    oldUri: oldUri,
    "data": {
              ws: network.replace(/\/+$/,"") + "/ontology/update/",
              method: "post",
              accept: "application/json",
              params: "ontology=" + escape(ontology) + "&function=updateEntityUri&parameters="
                + escape("oldUri=" + escape(oldUri)) + ";"
                + escape("newUri=" + escape(jQuery("#uriRenameInputText").val())) + ";"
                + escape("advancedIndexation=true")
                + "&reasoner=" + useReasoner
            },
    "success": function(data)
    {
      namedIndividualRenameSuccess();
    },
    "error": function(jqXHR, textStatus, error)
    {
      // The Ontology: Update web service does return an empty body, and not an empty JSON object.
      // This means that we have to rely on the HTTP status to know if the query is a success or 
      // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
      // string which returns a parse error.
      if(jqXHR.status == 200)
      {
        namedIndividualRenameSuccess();
      }
      else
      {      
        jQuery('#messagesContainer').fadeOut("fast", function()
        {
          jQuery('#messagesContainer').empty();

          jQuery("#renameButtonAjax").hide();
          jQuery("#renameButton").show();
        });

        var error = JSON.parse(jqXHR.responseText);

        var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

        jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
      }
    }
  });
}

function namedIndividualRenameSuccess()
{
  jQuery('#messagesContainer').append('<div class="successBox">URI successfully redefined</div>').hide().fadeIn(
  "fast");

  jQuery("#renameButtonAjax").hide();
  jQuery("#renameButton").show();

  var prefLabel = jQuery("#"+this.oldUri.replace(/[^a-zA-Z0-9]+/g, '')).text();
  
  jQuery("#"+this.oldUri.replace(/[^a-zA-Z0-9]+/g, '')).remove();
  
  jQuery('#iList').append('<li class="namedIndividualListItem" title="'+jQuery("#uriRenameInputText").val()+'" id="'+jQuery("#uriRenameInputText").val().replace(/[^a-zA-Z0-9]+/g, '')+'" onclick="selectIndividual(\''+jQuery("#uriRenameInputText").val()+'\');">'+prefLabel+'</li>');
  jQuery('#'+jQuery("#uriRenameInputText").val().replace(/[^a-zA-Z0-9]+/g, '')).data("uri", jQuery("#uriRenameInputText").val());
  
  createOrUpdateContextMenu();
  
  selectIndividual(jQuery("#uriRenameInputText").val());
}

function createOrUpdateContextMenu()
{
  jQuery(".namedIndividualListItem").contextMenu({
      menu: 'namedIndividualContentMenu'
  },
      function(action, el, pos) {
        
        switch(action)
        {
          case "createNewIndividual":
            createIntermediaryUriView();
          break;
          case "delete":
            deleteNamedIndividual(el.data("uri"));
          break;
          case "renameuri":
            createNamedIndividualUriView(el.data("uri"));
          break;
        }
  });     
}