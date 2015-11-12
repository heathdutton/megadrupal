/* Initialize the main osf_ontology page */

jQuery(document).ready(function() {

  // Make sure indexOf works in all browsers (specifically for IE)
  if(!Array.indexOf)
  {
    Array.prototype.indexOf = function(obj)
    {
      for(var i = 0; i < this.length; i++)
      {
        if(this[i] === obj)
        {
          return i;
        }
      }
      return -1;
    }
  }

  Object.size = function(obj)
  {
    var size = 0, key;

    for(key in obj)
    {
      if(obj.hasOwnProperty(key))
      {
        size++;
      }
    }
    return size;
  };
});

var resultset;

/* List all ontologies once the main, ontologies listing, page is loaded */
function readyListOntologies()
{
  // Run the ajax loading bar image
  jQuery('#ontologiesListContainer').append('<div class="loadingBar"><img src="' + osf_ontologyModuleFullPath
    + '/imgs/ajax-loading-bar.gif" width="220" height="19" /></div>');

  jQuery.ajax({
            type: "POST",
            url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
            data: ({
              ws: network.replace(/\/+$/,"") + "/ontology/read/",
              method: "post",
              accept: "application/json", 
              params: "function=getLoadedOntologies" +
                      "&parameters=" + escape("mode=descriptions")
            }),
            dataType: "json",
            success: function(msg)
            {

              // Remove the loading image
              jQuery('#ontologiesListContainer').empty();
              
              // Add the ontologies types sections
              jQuery("#ontologiesListContainer").append('<div id="ontologiesListContainer_local" style="display: none;">\
                                                      <h2>Local Ontologies</h2>\
                                                    </div>');
              jQuery("#ontologiesListContainer").append('<div id="ontologiesListContainer_reference" style="display: none;">\
                                                      <h2>Reference Ontologies</h2>\
                                                    </div>');
              jQuery("#ontologiesListContainer").append('<div id="ontologiesListContainer_administrative" style="display: none;">\
                                                      <h2>Administrative Ontologies</h2>\
                                                    </div>');
	      
              // Process the resultset
              resultset = new Resultset(msg);

              jQuery('#ontologiesListContainer').data("nb-ontologies", resultset.subjects.length);

              for(var i = 0; i < resultset.subjects.length; i++)
              {
                var ontologyModified =
                  resultset.subjects[i].getPredicateValues("http://purl.org/ontology/wsf#ontologyModified");

                if(ontologyModified.length > 0)
                {
                  if(ontologyModified[0].toUpperCase() == "TRUE")
                  {
                    ontologyModified = true;
                  }
                  else
                  {
                    ontologyModified = false;
                  }
                }
                else
                {
                  ontologyModified = false;
                }
                
                var ontologyType = resultset.subjects[i].getPredicateValues("http://purl.org/ontology/sco#ontologyType");
                var ontologyTypeContainerID = "ontologiesListContainer_local";
                
                if(ontologyType[0] != undefined)
                {
                  switch(ontologyType[0].uri)
                  {
                    case "http://purl.org/ontology/sco#localOntology":
                      ontologyTypeContainerID = "ontologiesListContainer_local";
                    break;
                    case "http://purl.org/ontology/sco#referenceOntology":
                      ontologyTypeContainerID = "ontologiesListContainer_reference";
                    break;
                    case "http://purl.org/ontology/sco#administrativeOntology":
                      ontologyTypeContainerID = "ontologiesListContainer_administrative";
                    break;
                  }
                }

                if(i % 2 == 0)
                {
                  jQuery("#"+ontologyTypeContainerID).append(
                    "<div class=\"list-even\"><input type=\"radio\" class=\"radioButton\" name=\"ontologiesListRadio\" id=\"radio_"
                      + i + "\" />" + resultset.subjects[i].getPrefLabel()
                      + (ontologyModified ? " <em><strong>(modified; not saved)</strong></em>" : "") 
                      + (isAdmin ? '<img src="' + osf_ontologyModuleFullPath + '/imgs/cog.png" class="ontologyEdit" onclick="editOntology('+i+')" title="Edit ontology description" />' : "") 
                      + (isAdmin ? '<img src="' + osf_ontologyModuleFullPath + '/imgs/lock.png" class="ontologyEdit" onclick="window.location = \'/admin/people/permissions#edit-create-ontology-'+hex_md5(resultset.subjects[i].uri)+'-'+sceid+'\'" title="Edit ontology permissions" style="padding-right: 5px;" />' : "") 
                      + "</div>");
                }
                else
                {
                  jQuery("#"+ontologyTypeContainerID).append(
                    "<div class=\"list-odd\"><input type=\"radio\" class=\"radioButton\" name=\"ontologiesListRadio\" id=\"radio_"
                      + i + "\" />" + resultset.subjects[i].getPrefLabel()
                      + (ontologyModified ? " <em><strong>(modified; not saved)</strong></em>" : "") 
                      + (isAdmin ? '<img src="' + osf_ontologyModuleFullPath + '/imgs/cog.png" class="ontologyEdit" onclick="editOntology('+i+')" title="Edit ontology description" />' : "") 
                      + (isAdmin ? '<img src="' + osf_ontologyModuleFullPath + '/imgs/lock.png" class="ontologyEdit" onclick="window.location = \'/admin/people/permissions#edit-create-ontology-'+hex_md5(resultset.subjects[i].uri)+'-'+sceid+'\'" title="Edit ontology permissions" style="padding-right: 5px;" />' : "") 
                      + "</div>");
                }
                
                if(ontologyType[0] != undefined)
                {                
                  if(ontologyType[0].uri != "http://purl.org/ontology/sco#administrativeOntology")    
                  {
                    jQuery("#"+ontologyTypeContainerID).show();
                  }
                  else
                  {
                    if(isAdmin)
                    {
                      jQuery("#"+ontologyTypeContainerID).show();
                    }
                  }
                }
                else
                {
                  jQuery("#"+ontologyTypeContainerID).show();
                }
                
                jQuery('#radio_'+i).click(function(){
                  var id = jQuery(this).attr("id").replace("radio_", "");
                  
                  redrawListColors(jQuery("#ontologiesListContainer"), id);
                  
                  jQuery("#deleteButtonInputUri").val(resultset.subjects[id].uri);
                  jQuery("#saveButtonInputUri").val(resultset.subjects[id].uri);
                  jQuery("#removeButtonInputUri").val(resultset.subjects[id].uri);
                  jQuery("#viewButtonIntputDataset").val(resultset.subjects[id].uri);
                  jQuery("#exportButtonIntputDataset").val(resultset.subjects[id].uri);                  
                });
                
                jQuery('#radio_'+i).parent().click(function(){
                  var id = jQuery(this).children(":first-child").attr("id").replace("radio_", "");
                  
                  jQuery('#radio_'+id).attr('checked', 'checked');
                  
                  redrawListColors(jQuery("#ontologiesListContainer"), id);
                  
                  jQuery("#deleteButtonInputUri").val(resultset.subjects[id].uri);
                  jQuery("#saveButtonInputUri").val(resultset.subjects[id].uri);
                  jQuery("#removeButtonInputUri").val(resultset.subjects[id].uri);
                  jQuery("#viewButtonIntputDataset").val(resultset.subjects[id].uri);
                  jQuery("#exportButtonIntputDataset").val(resultset.subjects[id].uri);                  
                });                

                // Check the first radio button
                if(i == 0)
                {
                  jQuery("#radio_0").attr("checked", "checked");
                  jQuery("#radio_0").parent().removeClass("list-even");
                  jQuery("#radio_0").parent().addClass("list-selected");
                  jQuery("#deleteButtonInputUri").val(resultset.subjects[0].uri);
                  jQuery("#saveButtonInputUri").val(resultset.subjects[0].uri);
                  jQuery("#removeButtonInputUri").val(resultset.subjects[0].uri);
                  jQuery("#viewButtonIntputDataset").val(resultset.subjects[0].uri);
                  jQuery("#exportButtonIntputDataset").val(resultset.subjects[0].uri);
                }
              }

              // Initialize all the selection radio button
              jQuery('input[name=ontologiesListRadio]:radio').change(function()
              {

                // Get the resultset key of the dataset selected
                var key = jQuery(this).attr("id").substr(6);

                // Change all the places where the URI of the selected ontology have to be changed (action buttons, etc).

                // Update the delete button's form
                jQuery("#deleteButtonInputUri").val(resultset.subjects[key].uri);
                jQuery("#saveButtonInputUri").val(resultset.subjects[key].uri);
                jQuery("#removeButtonInputUri").val(resultset.subjects[key].uri);
                jQuery("#viewButtonIntputDataset").val(resultset.subjects[key].uri);
                jQuery("#exportButtonIntputDataset").val(resultset.subjects[key].uri);
              });
            },
            error: function(jqXHR, textStatus, error)
            {
              displayError(textStatus + " :: " + jqXHR.responseText, "ontologiesListContainer");
            }
          });
}

function redrawListColors(containerElement, listItemID)
{
  for(var i = 0; i < containerElement.data("nb-ontologies"); i++)
  {
    if(i % 2 == 0)
    {    
      jQuery("#radio_"+i).parent().removeClass();
      
      if(i == listItemID)
      {
        jQuery("#radio_"+i).parent().addClass("list-selected");                                    
      }
      else
      {
        jQuery("#radio_"+i).parent().addClass("list-even");
      }
    }
    else
    {
      jQuery("#radio_"+i).parent().removeClass();
      
      if(i == listItemID)
      {
        jQuery("#radio_"+i).parent().addClass("list-selected");                                    
      }
      else
      {
        jQuery("#radio_"+i).parent().addClass("list-odd");
      }
    }      
  }
}

/* Display errors in the errors container */
function displayError(errorMsg, errorContainerID)
{
  jQuery("#" + errorContainerID).empty();
  jQuery("#" + errorContainerID).append('<div class="errorBox">' + errorMsg + '</div>');
}

/* Show/hide the import control to import new ontologies */
function toggleImportInput()
{
  if(jQuery("#importBox").is(':hidden'))
  {
    jQuery("#importBox").show();
  }
  else
  {
    jQuery("#importBox").hide();
  }
}

/* Display the ontology's properties to be modified */
function editOntology(id)
{
  jQuery('#ontologiesListContainer').fadeOut('slow', function()
  {
    jQuery('.actionViewButton').attr("disabled", "disabled");
    jQuery('.actionExportButton').attr("disabled", "disabled");
    jQuery('.actionCreateNewButton').attr("disabled", "disabled");
    jQuery('.actionSaveButton').attr("disabled", "disabled");
    jQuery('.actionRemoveButton').attr("disabled", "disabled");
    jQuery('.actionImportButton').attr("disabled", "disabled");
    jQuery('.actionCacheButton').attr("disabled", "disabled");
    jQuery('.actionReloadButton').attr("disabled", "disabled");

    jQuery('#ontologyEditContainer').data("ontology-id", id);
    
    jQuery('#ontologyEditContainer').fadeIn('slow', function()
    {
      var id = jQuery('#ontologyEditContainer').data("ontology-id");
           
      jQuery('#ontologyEditContainer').html(
        '<form method="POST">\
           <table id="ontologyDescriptionTable">\
             <tr>\
               <td>Ontology Name:</td>\
               <td class="ontologyDescriptionTd">\
                 <input type="text" id="ontologyName" name="ontologyName" class="form-text ontologyDescriptionInput" value="'+resultset.subjects[id].getPrefLabel()+'" />\
               </td>\
             </tr>\
             <tr>\
               <td>Ontology Description:</td>\
               <td class="ontologyDescriptionTd">\
                 <textarea style="width: 100%; height: 150px;" name="ontologyDescription" id="ontologyDescription" class="form-textarea ontologyDescriptionInput">\
                 '+resultset.subjects[id].getDescription()+'\
                 </textarea>\
               </td>\
             </tr>\
             <tr>\
               <td>Select Ontology\'s Type:</td>\
               <td>\
                 <select class="form-select" id="ontologyType" style="width: 100%">\
                  <option value="1">Local Ontology</option>\
                  <option value="2">Reference Ontology</option>\
                  <option value="3">Administrative Ontology</option>\
                 </select>\
               </td>\
             </tr>\
             <tr>\
               <td></td>\
               <td>\
                 <br />\
                 <center>\
                   <input class="form-submit" id="saveUpdatedOntologyButton" value="Save Ontology" />\
                 </center>\
               </td>\
             </tr>\
           </table>\
           <input type="hidden" name="updateOntologyDescription" />\
        </form>');
        
        var ontologyType = resultset.subjects[id].getPredicateValues("http://purl.org/ontology/sco#ontologyType");
        
        if(ontologyType[0] == undefined || !('uri' in ontologyType[0]))
        {
          jQuery('#ontologyType').val(1);
        }
        else
        {
          switch(ontologyType[0].uri)
          {
            case "http://purl.org/ontology/sco#localOntology":
              jQuery('#ontologyType').val(1);
            break;
            case "http://purl.org/ontology/sco#referenceOntology":
              jQuery('#ontologyType').val(2);
            break;
            case "http://purl.org/ontology/sco#administrativeOntology":
              jQuery('#ontologyType').val(3);
            break;
          }
        }
        
        jQuery('#ontologyName').change(function(){
          resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].rename(jQuery('#ontologyName').val());
        });
        
        jQuery('#ontologyDescription').change(function(){
          resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].setDescription(jQuery('#ontologyName').val());
        });
                
        jQuery('#ontologyType').change(function(){
          
          resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].removePredicateValues("http://purl.org/ontology/sco#ontologyType");
          
          switch(jQuery('#ontologyType').val())
          {
            case "1":
              resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].addAttributeValue("http://purl.org/ontology/sco#ontologyType", {uri: "http://purl.org/ontology/sco#localOntology"});
            break;
            case "2":
              resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].addAttributeValue("http://purl.org/ontology/sco#ontologyType", {uri: "http://purl.org/ontology/sco#referenceOntology"});
            break;
            case "3":
              resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].addAttributeValue("http://purl.org/ontology/sco#ontologyType", {uri: "http://purl.org/ontology/sco#administrativeOntology"});
            break;
          }
        });
        
        jQuery('#saveUpdatedOntologyButton').click(function (){
          jQuery.ajax({
            type: "POST",
            url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
            dataType: "json",
            "data": {
                      ws: network.replace(/\/+$/,"") + "/ontology/update/",
                      method: "post",
                      accept: "application/json",
                      params: "ontology=" + escape(resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].uri) + "&function=createOrUpdateEntity&parameters="
                        + escape("document=" + escape(resultset.saveN3(resultset.subjects[jQuery('#ontologyEditContainer').data("ontology-id")].uri))) + ";"
                        + escape("advancedIndexation=false") 
                        + "&reasoner=false"
                    },
            "success": function(data)
            {
              jQuery('.actionViewButton').attr("disabled", "");
              jQuery('.actionExportButton').attr("disabled", "");
              jQuery('.actionCreateNewButton').attr("disabled", "");
              jQuery('.actionSaveButton').attr("disabled", "");
              jQuery('.actionRemoveButton').attr("disabled", "");
              jQuery('.actionImportButton').attr("disabled", "");
              jQuery('.actionCacheButton').attr("disabled", "");
              jQuery('.actionReloadButton').attr("disabled", "");                    
              
              window.location.reload();
            },
            "error": function(jqXHR, textStatus, error)
            {
              // The Ontology: Update web service does return an empty body, and not an empty JSON object.
              // This means that we have to rely on the HTTP status to know if the query is a success or 
              // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
              // string which returns a parse error.
              if(jqXHR.status == 200)
              {
                jQuery('.actionViewButton').attr("disabled", "");
                jQuery('.actionExportButton').attr("disabled", "");
                jQuery('.actionCreateNewButton').attr("disabled", "");
                jQuery('.actionSaveButton').attr("disabled", "");
                jQuery('.actionRemoveButton').attr("disabled", "");
                jQuery('.actionImportButton').attr("disabled", "");
                jQuery('.actionCacheButton').attr("disabled", "");
                jQuery('.actionReloadButton').attr("disabled", "");                    
                
                window.location.reload();
              }
              else
              {                            
                if(jqXHR.status == 403)
                {                             
                  alert('You need to have administrator permissions to modify an ontology.');
                }
                else
                {
                  alert('An error occured; please record this issue to the system administrator.');
                }
              }             
            }
          })
        });
    })
  });  
}

/* Displays the controls to create a new ontology (like the title and description inputs etc.) */
function createNew()
{
  jQuery('#ontologiesListContainer').fadeOut('slow', function()
  {
    jQuery('.actionViewButton').attr("disabled", "disabled");
    jQuery('.actionExportButton').attr("disabled", "disabled");
    jQuery('.actionCreateNewButton').attr("disabled", "disabled");
    jQuery('.actionSaveButton').attr("disabled", "disabled");
    jQuery('.actionRemoveButton').attr("disabled", "disabled");
    jQuery('.actionImportButton').attr("disabled", "disabled");
    jQuery('.actionCacheButton').attr("disabled", "disabled");
    jQuery('.actionReloadButton').attr("disabled", "disabled");

    jQuery('#ontologyEditContainer').fadeIn('slow', function()
    {
      jQuery('#ontologyEditContainer').html(
        '<form method="POST">\
           <table id="ontologyDescriptionTable">\
             <tr>\
               <td>Ontology Name:</td>\
               <td class="ontologyDescriptionTd">\
                 <input type="text" id="ontologyName" name="ontologyName" class="form-text ontologyDescriptionInput" />\
               </td>\
             </tr>\
             <tr>\
               <td>Ontology Description:</td>\
               <td class="ontologyDescriptionTd">\
                 <input type="text" name="ontologyDescription" id="ontologyDescription" class="form-text ontologyDescriptionInput" />\
               </td>\
             </tr>\
             <tr>\
               <td>Ontology URI:</td>\
               <td class="ontologyDescriptionTd">\
                 <input type="text" id="ontologyUri" class="form-text ontologyUriInput" name="ontologyUri" />\
               </td>\
             </tr>\
             <tr>\
               <td></td>\
               <td>\
                 <br />\
                 <center>\
                   <input class="form-submit" type="submit" id="saveNewOntologyButton" value="Create New Ontology" />\
                 </center>\
               </td>\
             </tr>\
           </table>\
           <input type="hidden" name="createNewOntology" />\
        </form>');
    })
  });
}