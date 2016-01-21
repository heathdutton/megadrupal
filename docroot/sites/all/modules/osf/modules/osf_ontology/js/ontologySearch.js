var searchPage = 0;
var resultsPerPage = 10;
var searchQuery = "";

var filtersDatasets = [];
var filtersTypes = [];
var filtersAttributes = [];

var attributeValueFilters = [];

var checkedFiltersDatasets = [];
var checkedFiltersAttributes = [];
var checkedFiltersTypes = [];

var loadedOntologiesNames = [];

/** Send a search query when the search page is loaded */
function readySearch(query)
{
  // Run the ajax loading bar image
  jQuery('#resultsetContainer').append('<div class="loadingBar"><img src="' + osf_ontologyModuleFullPath
    + '/imgs/ajax-loading-bar.gif" width="220" height="19" /></div>');

  // get the names of each loaded ontologies.
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
              searchQuery = query;
              sendSearchQuery();

              // Process the resultset
              resultset = new Resultset(msg);

              for(var i = 0; i < resultset.subjects.length; i++)
              {
                loadedOntologiesNames[resultset.subjects[i].uri] = resultset.subjects[i].getPrefLabel();
              }
            },
            error: function(jqXHR, textStatus, error)
            {
              searchQuery = query;
              sendSearchQuery();
            }
          });
}

function submitSearchEnter(field, e)
{
  var keycode;

  if(window.event)
  {
    keycode = window.event.keyCode;
  }
  else if(e)
  {
    keycode = e.which;
  }
  else
  {
    return true;
  }

  if(keycode == 13)
  {
    search();

    return false;
  }
  else
  {
    return true;
  }
}

function search()
{
  var dataset = "all";
  
  if(typeof(initialDataset) == "undefined")
  {
    if(jQuery("#activeRadioButton").length > 0)
    {
      if(jQuery("#activeRadioButton:checked").length > 0)
      {
         dataset = jQuery('#viewButtonIntputDataset').val();
      }
    }      
  }
  else
  {
    dataset = initialDataset;
  }
  
  location.href = searchURL + '?query=' + escape(escape(jQuery("#searchBox").val())) + '&network=' + escape(OSFBaseURL)
    + '&dataset=' + dataset;
}

function sendSearchQuery()
{
  jQuery('#resultsetContainer').fadeTo('fast', 0.5, function(){});

  jQuery('#filtersContainer').fadeTo('fast', 0.5, function(){});

  // Prepare dataset filters
  $queryDatasetFilters = "all";

  if(checkedFiltersDatasets.length > 0)
  {
    $queryDatasetFilters = "";

    for(var i = 0; i < checkedFiltersDatasets.length; i++)
    {
      if(i + 1 == checkedFiltersDatasets.length)
      {
        $queryDatasetFilters += escape(checkedFiltersDatasets[i])
      }
      else
      {
        $queryDatasetFilters += escape(checkedFiltersDatasets[i]) + ";"
      }
    }
  }
  else if(initialDataset != "")
  {
    checkedFiltersDatasets.push(initialDataset)
    $queryDatasetFilters = escape(initialDataset);
  }

  // Prepare types filters
  $queryTypeFilters = escape("http://www.w3.org/2002/07/owl#Class") +
    ";" + escape("http://www.w3.org/2002/07/owl#ObjectProperty") +
    ";" + escape("http://www.w3.org/2002/07/owl#DatatypeProperty") +
    ";" + escape("http://www.w3.org/1999/02/22-rdf-syntax-ns#Property");

  if(checkedFiltersTypes.length > 0)
  {
    $queryTypeFilters = "";

    for(var i = 0; i < checkedFiltersTypes.length; i++)
    {
      if(i + 1 == checkedFiltersTypes.length)
      {
        $queryTypeFilters += escape(checkedFiltersTypes[i])
      }
      else
      {
        $queryTypeFilters += escape(checkedFiltersTypes[i]) + ";"
      }
    }
  }

  // Prepare attribute filters
  $queryAttributeFilters = "all";

  if(Object.size(attributeValueFilters) > 0)
  {
    $queryAttributeFilters = "";

    var i = 0;

    for(var uri in attributeValueFilters)
    {
      i++;

      if(i == Object.size(attributeValueFilters))
      {
        $queryAttributeFilters += escape(uri) + "::" + escape(attributeValueFilters[uri]);
      }
      else
      {
        $queryAttributeFilters += escape(uri) + "::" + escape(attributeValueFilters[uri]) + ";";
      }
    }
  }

  jQuery.ajax({
            type: "POST",
            url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
            data: {
              ws: network.replace(/\/+$/,"") + "/search/",
              method: "post",
              accept: "application/json", 
              params: "query=" + escape(searchQuery) +
                      "&datasets=" + $queryDatasetFilters +
                      "&attributes=" + $queryAttributeFilters +
                      "&page=" + (searchPage * resultsPerPage) +
                      "&items=" + resultsPerPage +
                      "&include_aggregates=true" + 
                      "&types=" + $queryTypeFilters
            },
            dataType: "json",
            success: function(msg)
            {
              jQuery('#resultsetContainer').empty();
              jQuery('#filtersContainer').empty();

              // Process the resultset
              resultset = new Resultset(msg);

              var nbResults = 0;

              // Get the aggregates to see how many results match this query
              for(var i = 0; i < resultset.subjects.length; i++)
              {
                if(jQuery.inArray("http://purl.org/ontology/aggregate#Aggregate", resultset.subjects[i].getTypes())
                  != -1)
                {
                  var values = resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#property");

                  for(var valueKey in values)
                  {
                    if(values[valueKey].uri == "http://rdfs.org/ns/void#Dataset")
                    {
                      nbResults += parseFloat(
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#count")[0]);
                    }
                  }
                }
              }

              if(nbResults == 0)
              {
                jQuery('#resultsetTitleContainer').html('A search for <strong>' + unescape(searchQuery)
                  + '</strong> returned 0 results');
              }
              else if(nbResults < 10)
              {
                jQuery('#resultsetTitleContainer').html('A search for <strong>' + unescape(searchQuery)
                  + '</strong> returned '
                  + nbResults + ' results');
              }
              else
              {
                jQuery('#resultsetTitleContainer').html('A search for <strong>' + unescape(searchQuery)
                  + '</strong> returned '
                  + (10 * (searchPage + 1)) + " of " + nbResults + " results")
              }

              // Process results to display in the results list
              var nb = 1;
              for(var i = 0; i < resultset.subjects.length; i++)
              {
                // Skip aggregates
                if(jQuery.inArray("http://purl.org/ontology/aggregate#Aggregate", resultset.subjects[i].getTypes())
                  != -1)
                {
                  continue;
                }

                var recordOntology = resultset.subjects[i].getDatasets();

                var targetUri = "";

                var type = resultset.subjects[i].getTypes();
                type = resultset.unprefixize(type[0]);

                switch(type)
                {
                  case "http://www.w3.org/2002/07/owl#DatatypeProperty":
                  case "http://www.w3.org/2002/07/owl#ObjectProperty":
                  case "http://www.w3.org/2002/07/owl#AnnotationProperty":
                    targetUri = "propertyUri=" + escape(resultset.subjects[i].uri);
                    break;

                  case "http://www.w3.org/2002/07/owl#Class":
                    targetUri = "classUri=" + escape(resultset.subjects[i].uri);
                    break;
                }

                if(i % 2 == 0)
                {
                  jQuery("#resultsetContainer").append("<div class=\"list-even\">"
                    + (nb + (searchPage * resultsPerPage)) + ". <a href=\"" + structModulesBaseUrl
                      + "ontology/view/?network=" + escape(network) + "&dataset=" + escape(recordOntology[0]) + "&"
                    + targetUri + "\">" + resultset.subjects[i].getPrefLabel() + "</a></div>");
                }
                else
                {
                  jQuery("#resultsetContainer").append("<div class=\"list-odd\">"
                    + (nb + (searchPage * resultsPerPage)) + ". <a href=\"" + structModulesBaseUrl
                      + "ontology/view/?network=" + escape(network) + "&dataset="
                    + escape(recordOntology[0]) + "&" + targetUri + "\">" + resultset.subjects[i].getPrefLabel()
                      + "</a></div>");
                }
                
                nb++;
              }

              // Display the pagination tool below the resultset container
              jQuery("#Pagination").pagination(nbResults, {
                        items_per_page: resultsPerPage,
                        num_display_entries: 5,
                        num_edge_entries: 3,
                        current_page: searchPage,
                        callback: handlePaginationClick
                      });


              //// Display Filters

              // Display Sources Filters
              filtersDatasets = [];

              for(var i = 0; i < resultset.subjects.length; i++)
              {
                if(jQuery.inArray("http://purl.org/ontology/aggregate#Aggregate", resultset.subjects[i].getTypes())
                  != -1)
                {
                  var values = resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#property");

                  for(var valueKey in values)
                  {
                    if(values[valueKey].uri == "http://rdfs.org/ns/void#Dataset")
                    {
                      var nb = parseFloat(
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#count")[0]);

                      var uri =
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#object")[0].uri;

                      var displayLabel = "";

                      if(loadedOntologiesNames[uri] != undefined)
                      {
                        var displayLabel = loadedOntologiesNames[uri] + " (" + nb + ")";
                      }
                      else
                      {
                        var displayLabel = uri + " (" + nb + ")";
                      }

                      filtersDatasets.push({
                                uri: uri,
                                displayLabel: displayLabel,
                                nb: nb
                              });
                    }
                  }
                }
              }

              displayFiltersDataset();

              // Display Types Filters
              filtersTypes = [];

              for(var i = 0; i < resultset.subjects.length; i++)
              {
                if(jQuery.inArray("http://purl.org/ontology/aggregate#Aggregate", resultset.subjects[i].getTypes())
                  != -1)
                {
                  var values = resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#property");

                  for(var valueKey in values)
                  {
                    if(values[valueKey].uri == "http://www.w3.org/1999/02/22-rdf-syntax-ns#type")
                    {
                      var nb = parseFloat(
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#count")[0]);

                      var uri =
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#object")[0].uri;

                      if(uri == "http://www.w3.org/2002/07/owl#Thing")
                      {
                        continue;
                      }

                      var displayLabel = "";

                      if(uri.lastIndexOf("#") != -1)
                      {
                        displayLabel = uri.substring(uri.lastIndexOf("#") + 1);
                      }
                      else if(uri.lastIndexOf("/") != -1)
                      {
                        displayLabel = uri.substring(uri.lastIndexOf("/") + 1);
                      }

                      displayLabel += " (" + nb + ")";
                      
                      filtersTypes.push({
                                uri: uri,
                                displayLabel: displayLabel,
                                nb: nb
                              });
                    }
                  }
                }
              }

              displayFiltersType();

              // Display Attributes Filters
              filtersAttributes = [];

              for(var i = 0; i < resultset.subjects.length; i++)
              {
                if(jQuery.inArray("http://purl.org/ontology/aggregate#Aggregate", resultset.subjects[i].getTypes())
                  != -1)
                {
                  var values = resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#property");

                  for(var valueKey in values)
                  {
                    if(values[valueKey].uri == "http://www.w3.org/1999/02/22-rdf-syntax-ns#Property")
                    {
                      var nb = parseFloat(
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#count")[0]);

                      var uri =
                        resultset.subjects[i].getPredicateValues("http://purl.org/ontology/aggregate#object")[0].uri;

                      var displayLabel = "";

                      if(uri.lastIndexOf("#") != -1)
                      {
                        displayLabel = uri.substring(uri.lastIndexOf("#") + 1);
                      }
                      else if(uri.lastIndexOf("/") != -1)
                      {
                        displayLabel = uri.substring(uri.lastIndexOf("/") + 1);
                      }

                      displayLabel += " (" + nb + ")";

                      filtersAttributes.push({
                                uri: uri,
                                displayLabel: displayLabel,
                                nb: nb
                              });
                    }
                  }
                }
              }

              displayFiltersAttribute();

              // Reabilitate the fading
              jQuery('#resultsetContainer').fadeTo('fast', 1, function(){});

              jQuery('#filtersContainer').fadeTo('fast', 1, function(){});
            },
            error: function(jqXHR, textStatus, error)
            {
              displayError(textStatus + " :: " + jqXHR.responseText, "resultsetContainer");
            }
          });
}

function displayFiltersDataset()
{
  /* Delete all previously inserted item */
  jQuery(".filtersTitleDataset").remove();
  jQuery(".filtersDataset").remove();

  jQuery("#filtersContainer").append('<div id="filtersTitleDataset" class="filtersTitleDataset">Sources</div>');
  jQuery("#filtersContainer").append('<div id="filtersDataset" class="filtersDataset"></div>');

  for(var i = 0; i < filtersDatasets.length; i++)
  {
    jQuery("#filtersDataset").append('<div id="filter_dataset_' + i + '" class="filtersDatasetItem"></div>');

    if(checkedFiltersDatasets.indexOf(filtersDatasets[i].uri) != -1)
    {
      jQuery("#filtersDataset").append('<input class="datasetCheckbox" type="checkbox" name="filter_dataset_checkbox_' + i
        + '" onclick="datasetFilterCheck(this);" title="Only display results from that source" checked>'
        + filtersDatasets[i].displayLabel);
    }
    else
    {
      jQuery("#filtersDataset").append('<input class="datasetCheckbox" type="checkbox" name="filter_dataset_checkbox_' + i
        + '" onclick="datasetFilterCheck(this);" title="Only display results from that source">'
        + filtersDatasets[i].displayLabel);
    }
  }
}

function datasetFilterCheck(checkbox)
{
  var index = checkbox.name.substring(checkbox.name.lastIndexOf("_") + 1);

  if(checkbox.checked) // This means that the checkbox has just been checked
  {
    checkedFiltersDatasets.push(filtersDatasets[index].uri);

    sendSearchQuery();
  }
  else // This means that the checkbox has just been un-checked
  {
    if(initialDataset == filtersDatasets[index].uri)
    {
      initialDataset = "";
    }

    removeByElement(checkedFiltersDatasets, filtersDatasets[index].uri);

    sendSearchQuery();
  }
}

function displayFiltersType()
{
  /* Delete all previously inserted item */
  jQuery(".filtersTitleType").remove();
  jQuery(".filtersType").remove();

  jQuery("#filtersContainer").append('<div id="filtersTitleType" class="filtersTitleType">Kinds</div>');
  jQuery("#filtersContainer").append('<div id="filtersType" class="filtersType"></div>');

  for(var i = 0; i < filtersTypes.length; i++)
  {
    jQuery("#filtersType").append('<div id="filter_type_' + i + '" class="filtersTypeItem"></div>');

    if(checkedFiltersTypes.indexOf(filtersTypes[i].uri) != -1)
    {
      jQuery("#filtersType").append('<input class="typeCheckbox" type="checkbox" name="filter_type_checkbox_' + i
        + '" onclick="typeFilterCheck(this);" title="Only display results of that kind" checked>'
        + '<span title="'+filtersTypes[i].uri+'">' + filtersTypes[i].displayLabel + '</span>');
    }
    else
    {
      jQuery("#filtersType").append('<input class="typeCheckbox" type="checkbox" name="filter_type_checkbox_' + i
        + '" onclick="typeFilterCheck(this);" title="Only display results of that kind">'
        + '<span title="'+filtersTypes[i].uri+'">' + filtersTypes[i].displayLabel + '</span>');
    }
  }
}

function typeFilterCheck(checkbox)
{
  var index = checkbox.name.substring(checkbox.name.lastIndexOf("_") + 1);

  if(checkbox.checked) // This means that the checkbox has just been checked
  {
    checkedFiltersTypes.push(filtersTypes[index].uri);

    sendSearchQuery();
  }
  else // This means that the checkbox has just been un-checked
  {
    removeByElement(checkedFiltersTypes, filtersTypes[index].uri);

    sendSearchQuery();
  }
}

function displayFiltersAttribute()
{
  /* Delete all previously inserted item */
  jQuery(".filtersTitleAttribute").remove();
  jQuery(".filtersAttribute").remove();
  jQuery(".filtersInputContainer").remove();

  jQuery("#filtersContainer").append('<div id="filtersTitleAttribute" class="filtersTitleAttribute">Attributes</div>');
  jQuery("#filtersContainer").append('<div id="filtersInputContainer" class="filtersInputContainer"></div>');
  jQuery("#filtersContainer").append('<div id="filtersAttribute" class="filtersAttribute"></div>');

  for(var i = 0; i < filtersAttributes.length; i++)
  {
    jQuery("#filtersAttribute").append('<div id="filter_attribute_' + i + '" class="filtersAttributeItem"></div>');

    if(attributeValueFilters[filtersAttributes[i].uri] == undefined)
    {
      jQuery("#filtersAttribute").append('<img onclick="addFilterValue(' + i + ');" id="attributeFilterImage_' + i
        + '" class="attributeFilterImage" src="' + osf_ontologyModuleFullPath
        + '/imgs/magnifier_zoom_in.png" title="Add a value to filter by" />');
    }
    else
    {
      jQuery("#filtersAttribute").append('<img onclick="removeAttributeValueFilter(' + i + ');" id="attributeFilterImage_'
        + i + '" class="attributeFilterImage" src="' + osf_ontologyModuleFullPath
        + '/imgs/magifier_zoom_out.png" title="Remove the value filter for that attribute" />');
    }

    if(checkedFiltersAttributes.indexOf(filtersAttributes[i].uri) != -1)
    {
      jQuery("#filtersAttribute").append('<span id="attributeFilterCheckboxText_' + i + '" title="'+filtersAttributes[i].uri+'">'
        + filtersAttributes[i].displayLabel + '</span>');
    }
    else
    {
      jQuery("#filtersAttribute").append('<span id="attributeFilterCheckboxText_' + i + '" title="'+filtersAttributes[i].uri+'">'
        + filtersAttributes[i].displayLabel + '</span>');
    }
  }
}

function addFilterValue(id)
{
  if(attributeValueFilters[filtersAttributes[id].uri] == undefined)
  {
    attributeValueFilters[filtersAttributes[id].uri] = "";
  }

  jQuery("#filtersInputContainer").append('<table class="attributeFilterInputTable" id="attributeFilterInputTable_' + id
    + '"><tr><td>Filter:</td> <td><input type="text" onKeyPress="return submitFilterEnter(this,event);" value="'
    + attributeValueFilters[filtersAttributes[id].uri] + '" id="filtersInputText_' + id
    + '" /></td><td><input type="submit" value="add" onclick="attributeValueFilter(' + id + ');" /></td></tr></table>')
  jQuery("#attributeFilterImage_" + id).attr("src", osf_ontologyModuleFullPath + "/imgs/magnifier.png");
  jQuery("#attributeFilterImage_" + id).attr("title", "Add the value to filter in the input text above");

  // Select all text and focus on that input text.
  jQuery("#filtersInputText_" + id).focus(function()
  {
    this.select();
  });

  // Add the auto-completion behavior to the filter box

  var acDatasets = "";

  for(var i = 0; i < filtersDatasets.length; i++)
  {
    if(checkedFiltersDatasets.length > 0)
    {
      if(checkedFiltersDatasets.indexOf(filtersDatasets[i].uri) != -1)
      {
        acDatasets = filtersDatasets[i].uri + ";";
      }
    }
    else
    {
      acDatasets = filtersDatasets[i].uri + ";";
    }
  }

  attributeValueFiltersWSString = "";

  for(var u in attributeValueFilters)
  {
    if(u != filtersAttributes[id].uri)
    {
      //      attributeValueFiltersWSString +=  urlencode(u) + "::" + urlencode('"'+attributeValueFilters[u]+'"') + ";";
      attributeValueFiltersWSString += urlencode(u) + "::" + urlencode(attributeValueFilters[u]) + ";";
    }
  }

  addWaitingIconNextTo('filtersInputText_' + id);

  filterAc = jQuery("#filtersInputText_" + id).autocomplete({
            serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
            structNetworkUrl: network.replace(/\/+$/,"") + "/search/",            
            minChars: 0,
            //delimiter: /(,|;)\s*/, // regex or character
            maxHeight: 400,
            width: 300,
            iconID: 'filtersInputText_' + id,
            zIndex: 9999,
            onSelect: function(value, data)
            {
              jQuery("#filtersInputText_" + id).val(value.substring(0, value.lastIndexOf(" (")));
            },
            deferRequestBy: 0, //miliseconds
            params: {
                      datasets: (acDatasets == '' ? 'all' : acDatasets),
                      attributes: urlencode(filtersAttributes[id].uri),
                      constrainedAttributes: attributeValueFiltersWSString,
                      page: 0,
                      items: 0,
                      include_aggregates: "true",
                      aggregate_attributes: filtersAttributes[id].uri,
                      types: "all"
                    },
            noCache: false, //default is false, set to true to disable caching
          });
 
  filterAc.enable();

  jQuery("#filtersInputText_" + id).focus();
}

function submitFilterEnter(field, e)
{
  var keycode;

  if(window.event)
  {
    keycode = window.event.keyCode;
  }
  else if(e)
  {
    keycode = e.which;
  }
  else
  {
    return true;
  }

  if(keycode == 13)
  {
    var inputID = field.id.substring(field.id.lastIndexOf("_") + 1);

    attributeValueFilter(inputID);

    return false;
  }
  else
  {
    return true;
  }
}

function attributeValueFilter(id)
{
  var filteringValue = jQuery("#filtersInputText_" + id).val();

  if(filteringValue == "")
  {
    jQuery("#attributeFilterInputTable_" + id).remove();
    jQuery("#attributeFilterImage_" + id).attr("src", osf_ontologyModuleFullPath + "/imgs/magnifier_zoom_in.png");
    jQuery("#attributeFilterImage_" + id).attr("title", "Add a value to filter by");
  }
  else
  {
    jQuery("#attributeFilterInputTable_" + id).remove();
    jQuery("#attributeFilterImage_" + id).attr("src", osf_ontologyModuleFullPath + "/imgs/magifier_zoom_out.png");
    jQuery("#attributeFilterImage_" + id).attr("title", "Remove the value filter for that attribute");

    attributeValueFilters[filtersAttributes[id].uri] = filteringValue;

    // Update the label of the checkbox
    var label = jQuery("#attributeFilterCheckboxText_" + id).html();
    var nbItems = label.substring(label.lastIndexOf("(") + 1, label.lastIndexOf(")"));

    if(label.lastIndexOf("=") == -1)
    {
      label = label.substring(0, label.lastIndexOf("(") - 1);
    }

    jQuery("#attributeFilterCheckboxText_" + id).html(label + " = " + filteringValue + " (" + nbItems + ")");

    sendSearchQuery();
  }
}

function removeAttributeValueFilter(id)
{
  jQuery("#attributeFilterImage_" + id).attr("src", osf_ontologyModuleFullPath + "/imgs/magnifier_zoom_in.png");
  jQuery("#attributeFilterImage_" + id).attr("title", "Add a value to filter by");

  delete attributeValueFilters[filtersAttributes[id].uri];

  sendSearchQuery();
}

function handlePaginationClick(new_page_index, pagination_container)
{
  if(this.current_page != new_page_index)
  {
    searchPage = new_page_index;
    sendSearchQuery();
  }

  return false;
}

function removeByElement(arrayName, arrayElement)
{
  for(var i = 0; i < arrayName.length; i++)
  {
    if(arrayName[i] == arrayElement)
    {
      arrayName.splice(i, 1);
    }
  }
}

function urlencode(str)
{
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
  return encodeURIComponent(str).replace(/!/g, '%21').replace(/'/g, '%27').replace(/\(/g, '%28').replace(/\)/g,
    '%29').replace(/\*/g, '%2A').replace(/%20/g, '+');
}

function addWaitingIconNextTo(nextToElement)
{
  if(jQuery('#' + nextToElement).next().attr("src") == undefined
    || jQuery('#' + nextToElement).next().attr("src").search("ajax-loader-icon.gif") == -1)
  {
    jQuery('#' + nextToElement).after('<img src="' + osf_ontologyModuleFullPath
      + '/imgs/ajax-loader-icon.gif" class="ajax-waiting-icon" />');
  }
}

function removeWaitingIconNextTo(nextToElement)
{
  if(jQuery('#' + nextToElement).next().attr("src") != undefined
    && jQuery('#' + nextToElement).next().attr("src").search("ajax-loader-icon.gif") != -1)
  {
    jQuery('#' + nextToElement).next().remove();
  }
}