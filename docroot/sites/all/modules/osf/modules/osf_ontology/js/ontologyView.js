var classTreeNodeBinder = [];
var ontology = "";
var network = "";
var reloadTree = false;
var extendedToAllOntologies = true;
var importedFromExternalOntology = [];

// The resultset of the entity currently being created/modified
var currentEntityResultset;

var removedSuperEntityOf = [];

var loadedOntologiesNames = [];
var existingTemplates = [];

var propertiesTreeCreated = false;
var individualsListCreated = false;

var currentView = "classes";

var lastSelectedClassNode = "httpwwww3org200207owlThing";
var lastSelectedPropertyNode = "httpwwww3org200207owlTopProperty";

var searchAc;

var initiallyOpenedNodeIdReference = "";

var baseOntologyUri = "";

/** Prepate the page when it is loaded */
function readyView(dataset, netwk, classUri, propertyUri, niUri)
{
  jQuery("#classesTree").data("classUri", classUri);
  jQuery("#classesTree").data("propertyUri", propertyUri);
  jQuery("#classesTree").data("niUri", niUri);

  searchAc = jQuery('#searchBox').autocomplete({
            serviceUrl: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
            structNetworkUrl: netwk.replace(/\/+$/,"") + "/search/",
            minChars: 3,
            //delimiter: /(,|;)\s*/, // regex or character
            maxHeight: 400,
            width: 300,
            iconID: "searchButtonImage",
            zIndex: 9999,
            onSelect: function(value, data)
            {
              jQuery("#searchBox").data("uri", data);

              switch(currentView)
              {
                case "classes":
                  jQuery("#classesTree").jstree("search", value.substring(0, value.search("&nbsp;&nbsp;&nbsp;")));
                break;

                case "properties":
                  jQuery("#propertiesTree").jstree("search", value.substring(0, value.search("&nbsp;&nbsp;&nbsp;")));
                break;
              }
            },
            deferRequestBy: 0, //miliseconds
            params: {
                      datasets: dataset,
                      attributes: "http://purl.org/ontology/iron#prefLabel",
                      page: 0,
                      items: 20,
                      include_aggregates: "false",
                      types: "http://www.w3.org/2002/07/owl#Class"
                    },
            noCache: false, //default is false, set to true to disable caching
          });

  searchAc.enable();

  ontology = dataset;

  network = netwk;

  if(niUri != "")
  {
    toggleIndividualsView();
  }  
  
  var uri = "http://www.w3.org/2002/07/owl#Thing";

  classTreeNodeBinder[uri.replace(/[^a-zA-Z0-9]+/g, '')] = uri;
  
  initiallyOpenedNodeIdReference = uri.replace(/[^a-zA-Z0-9]+/g, '');
  
  var jstreeStructure = {
      core: {
                initially_open: [uri.replace(/[^a-zA-Z0-9]+/g, '')],
                strings: {
                          loading: "Loading ...",
                          new_node: "New class"
                        },
              },
      "json_data": {
                "data": [
                  {
                            "data": "Thing",
                            "attr": {
                                      id: uri.replace(/[^a-zA-Z0-9]+/g, '')
                                    },
                            "state": "closed"
                          }
                ],
                "ajax": {               
                          type: "POST",
                          url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                          dataType: "json",
                          "data": function(n)
                          {
                            // Here we take the node's ID to compose the query to send to OSF
                            return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                   "&method=" + "post" +
                                   "&accept=" + escape("application/json") +
                                   "&params=" + 
                                     escape( "function=getSubClasses" + 
                                             "&reasoner=" + useReasoner + 
                                             "&ontology=" + escape(dataset) + 
                                             "&parameters=" + escape("mode=hierarchy") + ";" 
                                                            + escape("uri=" + classTreeNodeBinder[n[0].id]) + ";" 
                                                            + escape("direct=1")));
                          },      
                          "success": function(data)
                          {

                            jQuery('#messagesContainer').fadeOut("slow", function()
                            {
                              jQuery('#messagesContainer').empty();
                            });

                            // Here we read the resultset, and compose the JSON object to feed to jsTree
                            var resultset = new Resultset(data);

                            var nodes = [];

                            for(var i = 0; i < resultset.subjects.length; i++)
                            {
                              classTreeNodeBinder[resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g,
                                '')] = resultset.subjects[i].uri;

                              var nodeType = "default";

                              switch(resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g, ''))
                              {
                                case "httpwwww3org200207owlThing":
                                case "httpwwww3org200207owlNothing":
                                case "httpwwww3org200207owlDatatypeProperty":
                                case "httpwwww3org200207owlClass":
                                case "httpwwww3org200207owlObjectProperty":
                                case "httpwwww3org200207owlAnnotationProperty":
                                  nodeType = "internalClasses";
                                  break;
                              }

                              // Check if it is a super class of another class. If not, then it is a leaf
                              // node in the tree control.
                              if(resultset.subjects[i].getPredicateValues(
                                "http://purl.org/ontology/sco#hasSubClasses").length <= 0)
                              {
                                nodes.push({
                                          data: resultset.subjects[i].getPrefLabel(),
                                          attr: {
                                                    id: resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g, ''),
                                                    title: resultset.subjects[i].uri,
                                                    rel: nodeType
                                                  },
                                        });
                              }
                              else
                              {
                                nodes.push({
                                          data: resultset.subjects[i].getPrefLabel(),
                                          attr: {
                                                    id: resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g, ''),
                                                    title: resultset.subjects[i].uri,
                                                    rel: nodeType
                                                  },
                                          state: "closed"
                                        });
                              }
                            }

                            return(nodes);
                          },
                          "error": function(jqXHR, textStatus, error)
                          {
                            jQuery('#messagesContainer').fadeOut("fast", function()
                            {
                              jQuery('#messagesContainer').empty();
                            });

                            var error = JSON.parse(jqXHR.responseText);

                            var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                            jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg
                              + '</div>').hide().fadeIn("fast");
                          }
                        }
              },
      "search": {
                "ajax": {
                          type: "POST",
                          url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                          data: function(query)
                          {
                            return("ws=" + network.replace(/\/+$/,"") + "/search/" +
                                   "&method=post" +
                                   "&accept=" + escape("application/json") + 
                                   "&params=" + escape( "query=" + "\"" + query + "\"" +
                                                        "&datasets=" + escape(dataset) +
                                                        "&attributes=" + "all" +
                                                        "&page=" + "0" +
                                                        "&items=" + "20" +
                                                        "&include_aggregates=false" +
                                                        "&types=" + "http://www.w3.org/2002/07/owl#Class"));
                          },
                          success: function(data)
                          {

                            jQuery('#messagesContainer').fadeOut("fast", function()
                            {
                              jQuery('#messagesContainer').empty();
                            });

                            resultset = new Resultset(data);

                            var root = "http://www.w3.org/2002/07/owl#Thing";
                            var nodes = ["#" + root.replace(/[^a-zA-Z0-9]+/g, '')];

                            for(var i = 0; i < resultset.subjects.length; i++)
                            {
                              classTreeNodeBinder[resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g,
                                '')] = resultset.subjects[i].uri;
                              nodes.push("#" + resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g, ''));
                            }

                            if(jQuery("#searchBox").data("uri") != undefined && jQuery("#searchBox").data("uri").uri != "")
                            {                                            
                              var superClasses = jQuery.ajax({
                                        type: "POST",
                                        url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                                        dataType: "json",
                                        data: ({
                                                  ws: network.replace(/\/+$/,"") + "/ontology/read/",
                                                  method: "post",
                                                  accept: "application/json", 
                                                  params: "function=getSuperClasses" +   
                                                          "&reasoner=" + useReasoner +
                                                          "&ontology=" + escape(ontology) +
                                                          "&parameters=" + escape("mode=descriptions") + ";" + 
                                                                           escape("uri=" + jQuery("#searchBox").data("uri").uri) + ";" + 
                                                                           escape("direct=0")
                                                }),
                                        async: false
                                      }).responseText;

                              //check if we got an error
                              var jsonResponseText = JSON.parse(superClasses);

                              if('level' in jsonResponseText && 'debugInformation' in jsonResponseText)
                              {
                                jQuery('#messagesContainer').fadeOut("fast", function()
                                {
                                  jQuery('#messagesContainer').empty();
                                });

                                var errorMsg = '[' + jsonResponseText.id + '] ' + jsonResponseText.name + ': '
                                  + jsonResponseText.description;

                                jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg
                                  + '</div>').hide().fadeIn("fast");
                                return;
                              }

                              resultsetSuperClasses = new Resultset(jsonResponseText);

                              var pathClasses = getClassesTreeOrderedPath(resultsetSuperClasses,
                                "http://www.w3.org/2002/07/owl#Thing");
                              var path = [];

                              for(var i = 0; i < pathClasses.length; i++)
                              {
                                classTreeNodeBinder[pathClasses[i].replace(/[^a-zA-Z0-9]+/g,
                                  '')] = pathClasses[i];
                                path.push("#" + pathClasses[i].replace(/[^a-zA-Z0-9]+/g, ''));
                              }

                              return(path);
                            }
                          },
                          "error": function(jqXHR, textStatus, error)
                          {
                            jQuery('#messagesContainer').fadeOut("fast", function()
                            {
                              jQuery('#messagesContainer').empty();
                            });

                            var error = JSON.parse(jqXHR.responseText);

                            var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                            jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg
                              + '</div>').hide().fadeIn("fast");
                          },                          
                          dataType: "json"
                        }
              },
      "ui": {
                "select_limit": 1,
                "selected_parent_close": "select_parent",
                "initially_select": [uri.replace(/[^a-zA-Z0-9]+/g, '')]
              },
      "types": {
                "types": {
                          "internalClasses": {
                                    "valid_children": ["internalClasses", "default"],
                                    "icon": {
                                              "image": osf_ontologyModuleFullPath + '/imgs/folder_wrench.png'
                                            }
                                  },
                          "default": {
                                    "valid_children": ["internalClasses", "default"],
                                    "icon": {
                                              "image": osf_ontologyModuleFullPath + '/imgs/folder.png'
                                            }
                                  }
                        }
              },

      /* Case insensitive sort function */
      "sort": function(a, b)
      {
        return(this.get_text(a).toLowerCase() > this.get_text(b).toLowerCase() ? 1 : -1);
      },
    } 
  
  if(isAdmin)
  {
    jstreeStructure["plugins"] = ["themes", "json_data", "search", "ui", "contextmenu", "crrm", "hotkeys", "dnd", "types", "sort"];
    
    jstreeStructure["contextmenu"] = {
                "items": {
                          "create": {
                                    // The item label
                                    "label": "Create new class...",
                                    // The function to execute upon a click
                                    "action": function(obj)
                                    {
                                      this.create(obj);
                                    },    
                                    // All below are optional
                                    "_disabled": false, // clicking the item won't do a thing
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/folder_add.png',
                                    "separator_before": false, // Insert a separator before the item
                                    "separator_after": false
                                  },
                          "ccp": false,
                          "remove": {
                                    // The item label
                                    "label": "Delete",
                                    // The function to execute upon a click
                                    "action": function(obj)
                                    {

                                      this.remove(obj);
                                    },
                                    // All below are optional
                                    "_disabled": false, // clicking the item won't do a thing
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/folder_delete.png',
                                    "separator_before": false, // Insert a separator before the item
                                    "separator_after": false   // Insert a separator after the item
                                  },
                          "rename": {
                                    // The item label
                                    "label": "Rename",
                                    // The function to execute upon a click
                                    "action": function(obj)
                                    {

                                      this.rename(obj);
                                    },
                                    // All below are optional
                                    "_disabled": false, // clicking the item won't do a thing
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/folder_edit.png',
                                    "separator_before": false, // Insert a separator before the item
                                    "separator_after": false   // Insert a separator after the item
                                  },
                          "renameUri": {
                                    id: "renameUri",
                                    label: "Rename URI...",
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/folder_wrench.png',
                                    visible: function(node, treeObj)
                                    {
                                      if(node.length != 1)
                                      {
                                        return false;
                                      }
                                      else
                                      {
                                        return treeObj.check("creatable", node);
                                      }
                                    },
                                    action: function(node, treeObj)
                                    {
                                      createUriView();
                                    }
                                  }
                        }
              };
    jstreeStructure["dnd"] =  {
                "drop_finish": function(data)
                {
                  if(data.r.attr("id").search("structureSuperClass")
                    != -1 || data.r.attr("id").search("structureSubClass")
                      != -1 || data.r.attr("id").search("structureEquivalentClass")
                        != -1 || data.r.attr("id").search("structureDisjointWith") != -1)
                  {
                    data.r.data("value-uri", data.o.attr("title"));
                    data.r.val(data.o.text().replace(/^\s+|\s+$/g, ''));

                    addInputAutocomplete(data.r.data("attr-prefix"), data.r.data("element-id"),
                      data.r.data("jsdrop"), data.r.data("search-filter-attributes"),
                      data.r.data("search-filter-types"));

                    if(data.r.data("force-reload"))
                    {
                      reloadTree = true;
                    }
                  }
                },
                "drag_check": function(data)
                {
                  if(data.r.attr("id") == "httpwwww3org200207owlThing")
                  {
                    return false;
                  }
                  return {
                            after: false,
                            before: false,
                            inside: true
                          };
                }
              };
      jstreeStructure["crrm"] = {
                "move": {
                          "default_position": "inside",
                          "check_move": function(m)
                          {
                            return(true)
                          }
                        }
              };    
    
    
  }
  else
  {
    jstreeStructure["plugins"] = ["themes", "json_data", "search", "ui", "hotkeys", "types", "sort"]
  }
  
  

  jQuery("#classesTree").bind("loaded.jstree", function(event, data)
  {
    jQuery("#httpwwww3org200207owlThing").attr("title", "http://www.w3.org/2002/07/owl#Thing");

    jQuery.jstree._focused().set_type("internalClasses", jQuery("#httpwwww3org200207owlThing"));

    // get the names of each loaded ontologies.
    jQuery.ajax({                          
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              data: ({
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=getLoadedOntologies" +   
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
                                "&parameters=" + escape("mode=descriptions")
                      }),              
              dataType: "json",
              success: function(msg)
              {

                // Process the resultset
                resultset = new Resultset(msg);

                for(var i = 0; i < resultset.subjects.length; i++)
                {
                  loadedOntologiesNames[resultset.subjects[i].uri] = resultset.subjects[i].getPrefLabel();
                  
                  if(resultset.subjects[i].uri == ontology)
                  {
                    baseOntologyUri = resultset.subjects[i].getPredicateValues("http://www.w3.org/2000/01/rdf-schema#isDefinedBy")[0].uri;                    
                  }
                }

                // Change the name of the ontology in the breadcrumb
                jQuery("#breadCrumbOntologyName").text(loadedOntologiesNames[ontology]);
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

    // get the name of all existing OSF for Drupal templates
    jQuery.ajax({
              type: "GET",
              url: osf_ontologyModuleFullPath.replace(/\/+$/,"") + "/getTemplates.php",
              dataType: "json",
              success: function(msg)
              {
                existingTemplates = msg.templates;
              },
              error: function(jqXHR, textStatus, error)
              {
                jQuery('#messagesContainer').fadeOut("fast", function()
                {
                  jQuery('#messagesContainer').empty();
                });

                jQuery('#messagesContainer').append(
                  '<div class="errorBox">Can\'t get the list of available OSF for Drupal templates</div>').hide().fadeIn(
                  "fast");
              }
            });
  }).jstree(jstreeStructure).bind("move_node.jstree", jQuery.proxy(function(e, data)
  {

    var draggedConceptUri = classTreeNodeBinder[data.rslt.o.attr("id")];
    var parentDraggedConceptUri = classTreeNodeBinder[data.rslt.op.attr("id")];
    var targetParentConceptUri = classTreeNodeBinder[data.rslt.cr.attr("id")];

    jQuery.ajax({
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              dataType: "json",
              newName: data.rslt.new_name,
              "data": {
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=getClass" + 
                                "&reasoner=" + useReasoner + 
                                "&ontology=" + escape(dataset) + 
                                "&parameters=" + escape("uri=" + draggedConceptUri)
                      },
              "success": function(data)
              {
                jQuery('#messagesContainer').fadeOut("slow", function()
                {
                  jQuery('#messagesContainer').empty();
                });

                // Here we read the resultset, and compose the JSON object to feed to jsTree
                resultset = new Resultset(data);

                resultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superClassOf");

                resultset.subjects[0].removePredicateValue("http://www.w3.org/2000/01/rdf-schema#subClassOf",
                  parentDraggedConceptUri);
                resultset.subjects[0].addAttributeValue("http://www.w3.org/2000/01/rdf-schema#subClassOf", {
                          uri: targetParentConceptUri
                        });

                jQuery.ajax({
                          type: "POST",
                          url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                          dataType: "json",
                          "data": {
                                    ws: network.replace(/\/+$/,"") + "/ontology/update/",
                                    method: "post",
                                    accept: "application/json",
                                    params: "ontology=" + escape(ontology) + "&function=createOrUpdateEntity&parameters="
                                      + escape("document=" + escape(resultset.saveN3())) + ";"
                                      + escape("advancedIndexation=true") 
                                      + "&reasoner="+useReasoner
                                  },
                          "success": function(data)
                          {
                            classMoveSuccess();
                          },
                          "error": function(jqXHR, textStatus, error)
                          {
                            // The Ontology: Update web service does return an empty body, and not an empty JSON object.
                            // This means that we have to rely on the HTTP status to know if the query is a success or 
                            // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
                            // string which returns a parse error.
                            if(jqXHR.status == 200)
                            {
                              classMoveSuccess();
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
                        })
              },
              "error": function(jqXHR, textStatus, error)
              {
                jQuery('#messagesContainer').fadeOut("fast", function()
                {
                  jQuery('#messagesContainer').empty();
                });

                var error = JSON.parse(jqXHR.responseText);

                var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
              }
            })
  })).bind("remove.jstree", function(e, data)
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
                        params: "ontology=" + escape(ontology) + "&function=deleteClass&parameters="
                          + escape("uri=" + classTreeNodeBinder[data.args[0][0].id])
                      },
              "success": function(data)
              {
                classDeleteSuccess();
              },
              "error": function(jqXHR, textStatus, error)
              {
                // The Ontology: Update web service does return an empty body, and not an empty JSON object.
                // This means that we have to rely on the HTTP status to know if the query is a success or 
                // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
                // string which returns a parse error.
                if(jqXHR.status == 200)
                {
                  classDeleteSuccess();
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
            })
  }).bind("create.jstree", function(e, data)
  {

    var name = data.args[0].text().replace(/^\s+|\s+$/g, '');
    name = name.substring(0, name.search(/[\s]/));

    currentEntityResultset = new Resultset({
              prefixes: {
                        "owl": "http://www.w3.org/2002/07/owl#",
                        "rdf": "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
                        "rdfs": "http://www.w3.org/2000/01/rdf-schema#",
                        "wsf": "http://purl.org/ontology/wsf#",
                        "aggr": "http://purl.org/ontology/aggregate#",
                        "ns0": "http://www.w3.org/2004/02/skos/core#",
                        "ns1": "http://umbel.org/umbel#"
                      },
              resultset: {
                        subject: [
                          {
                                    uri: "",
                                    type: "owl:Class",
                                    predicate: [
                                      {
                                                "rdfs:label": data.rslt.name
                                              },
                                      {
                                                "rdfs:subClassOf": {
                                                          "uri": classTreeNodeBinder[data.args[0].attr("id")],
                                                          "reify": [
                                                            {
                                                                      "type": "wsf:objectLabel",
                                                                      "value": name
                                                                    }
                                                          ]
                                                        }
                                              }
                                    ]
                                  }
                        ]
                      }
            });

    createClassView();
  }).bind("rename.jstree", function(e, data)
  {
    if(data.rslt.new_name == data.rslt.old_name)
    {
      // If the name didn't change, just continue without doing anything else.
      return;
    }

    // Get the record's description
    jQuery.ajax({
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              dataType: "json",
              newName: data.rslt.new_name,
              "data": {                         
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=getClass" +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
                                "&parameters=" + escape("uri=" + classTreeNodeBinder[data.args[0][0].id])
                      },
              "success": function(data)
              {
                resultset = new Resultset(data);

                resultset.subjects[0].rename(this.newName);

                // Now update the record's description
                jQuery.ajax({
                          type: "POST",
                          url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                          dataType: "json",
                          "data": {
                                    ws: network.replace(/\/+$/,"") + "/ontology/update/",
                                    method: "post",
                                    accept: "application/json",
                                    params: "ontology=" + escape(ontology) + "&function=createOrUpdateEntity&parameters="
                                      + escape("document=" + escape(resultset.saveN3())) + ";"
                                      + escape("advancedIndexation=true")
                                      + "&reasoner=" + useReasoner
                                  },
                          "success": function(data)
                          {
                            classRenameSuccess();
                          },
                          "error": function(jqXHR, textStatus, error)
                          {
                            // The Ontology: Update web service does return an empty body, and not an empty JSON object.
                            // This means that we have to rely on the HTTP status to know if the query is a success or 
                            // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
                            // string which returns a parse error.
                            if(jqXHR.status == 200)
                            {
                              classRenameSuccess();
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
                        })
              },
              "error": function(jqXHR, textStatus, error)
              {
                jQuery('#messagesContainer').fadeOut("fast", function()
                {
                  jQuery('#messagesContainer').empty();
                });

                var error = JSON.parse(jqXHR.responseText);

                var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
              }
            })
  }).bind('select_node.jstree', function(e, data)
  {
    var id = data.rslt.obj[0].attributes.id.nodeValue;
    
    /* 
      Here we are initializing the tree, once the first node is selected.
      This initialization is related to the pre-selection of a class, or a predicate, in the tree
      (Normally happen when a user clicks on a result of a search query.)
    */
    if(id == initiallyOpenedNodeIdReference)
    {
      if(jQuery("#classesTree").data("classUri") != "")
      {
        // Get the description of the target class to focus on
        var targetClass = jQuery.ajax({
                  type: "POST",
                  url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                  dataType: "json",
                  data: ({                            
                            ws: network.replace(/\/+$/,"") + "/ontology/read/",
                            method: "post",
                            accept: "application/json", 
                            params: "function=getClass" +
                                    "&reasoner=" + useReasoner +
                                    "&ontology=" + escape(ontology) +
                                    "&parameters=" + escape("uri=" + jQuery("#classesTree").data("classUri"))
                          }),
                  async: false
                }).responseText;

        //check if we got an error
        var targetClass = JSON.parse(targetClass);

        if('level' in targetClass && 'debugInformation' in targetClass)
        {
          jQuery('#messagesContainer').fadeOut("fast", function()
          {
            jQuery('#messagesContainer').empty();
          });

          var errorMsg = '[' + targetClass.id + '] ' + targetClass.name + ': ' + targetClass.description;

          jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
          return;
        }

        targetClass = new Resultset(targetClass);

        var prefLabel = targetClass.subjects[0].getPrefLabel();

        // Do a search on the preffered label of that class
        jQuery("#searchBox").data("uri", {
                  uri: jQuery("#classesTree").data("classUri")
                });

        jQuery("#classesTree").jstree("search", prefLabel);
      }      

      if(jQuery("#classesTree").data("propertyUri") != "")
      {
        // Get the description of the target property to focus on
        var targetProperty = jQuery.ajax({
                  type: "POST",
                  url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                  dataType: "json",
                  data: ({
                            ws: network.replace(/\/+$/,"") + "/ontology/read/",
                            method: "post",
                            accept: "application/json", 
                            params: "function=getProperty" +
                                    "&reasoner=" + useReasoner +
                                    "&ontology=" + escape(ontology) +
                                    "&parameters=" + escape("uri=" + jQuery("#classesTree").data("propertyUri"))
                          }),
                  async: false
                }).responseText;

        // Switch to the property tree view control
        togglePropertiesView();

        // Do a search on the preffered label of that property
        var targetProperty = JSON.parse(targetProperty);

        if('level' in targetProperty && 'debugInformation' in targetProperty)
        {
          jQuery('#messagesContainer').fadeOut("fast", function()
          {
            jQuery('#messagesContainer').empty();
          });

          var errorMsg = '[' + targetProperty.id + '] ' + targetProperty.name + ': ' + targetProperty.description;

          jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
          return;
        }

        targetProperty = new Resultset(targetProperty);

        var prefLabel = targetProperty.subjects[0].getPrefLabel();

        // Do a search on the preffered label of that class
        jQuery("#searchBox").data("uri", {
                  uri: jQuery("#classesTree").data("propertyUri")
                });

        jQuery("#propertiesTree").jstree("search", prefLabel);
      }      
    }

    lastSelectedClassNode = id;

    var uri = classTreeNodeBinder[id];

    jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function(){});

    jQuery.ajax({
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              dataType: "json",
              "data": {
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=getClass" +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
                                "&parameters=" + escape("uri=" + uri)
                      },
              "success": function(data)
              {

                // Here we read the resultset, and compose the JSON object to feed to jsTree
                currentEntityResultset = new Resultset(data);

                createClassView();
              },
              "error": function(jqXHR, textStatus, error)
              {
                jQuery('#messagesContainer').fadeOut("fast", function()
                {
                  jQuery('#messagesContainer').empty();
                });

                var error = JSON.parse(jqXHR.responseText);
                
                // Some ontologies complains that owl:Thing is not existing, so we make sure that if
                // this happens that we *don't* return any error to the user
                if(error.id != 'WS-ONTOLOGY-READ-205' || uri != 'http://www.w3.org/2002/07/owl#Thing') 
                {
                  var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                  jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
                }
              }
            })
  })
}

function createUriView()
{
  var uri = currentEntityResultset.subjects[0].uri;

  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function(){});

  // Empty the record section.
  jQuery("#ontologyViewColumRecord").empty();

  // Generate the URI Rename section table
  jQuery("#ontologyViewColumRecord").append('<table id="uriRenameTable">');
  jQuery("#uriRenameTable").append('<tr><td id="uriRenameTableHeader" colspan="2">Rename URI</td></tr>');

  jQuery("#uriRenameTable").append(
    '<tr class="uriRenameRow"><td class="uriRenameText" valign="top">URI:</td><td valign="top"><input id="uriRenameInputText" class="uriRenameInputText" type="text" value="'
    + uri + '" /></td></tr>');

  jQuery('#ontologyViewColumRecord').append(
    '<div class="buttonsContainer"><input title="Rename" id="renameButton" type="submit" value="Rename" onclick="renameUri(\''
    + uri + '\');" class="renameButton" /></div>');

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function(){});
}

function createIntermediaryUriView()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function(){});

  // Empty the record section.
  jQuery("#ontologyViewColumRecord").empty();

  // Generate the URI Rename section table
  jQuery("#ontologyViewColumRecord").append('<table id="uriRenameTable">');
  jQuery("#uriRenameTable").append('<tr><td id="uriRenameTableHeader" colspan="2">Name URI</td></tr>');

  /*jQuery("#uriRenameTable").append(
    '<tr class="uriRenameRow"><td class="uriRenameText" valign="top" style="width: 100px;">URI ending:</td><td valign="top"><input id="uriRenameInputText" class="uriRenameInputText" type="text" value="" /></td></tr>\
     <tr class="uriRenameRow"><td colspan="2"><em>Note: the URI ending shouldn\'t include the namespace. The namespace that will be used is the one of the ontology.</em></td></tr>');*/
  jQuery("#uriRenameTable").append(
    '<tr class="uriRenameRow"><td class="uriRenameText" valign="top" style="width: 100px;">URI:</td><td valign="top"><input id="uriRenameInputText" class="uriRenameInputText" type="text" value="'+baseOntologyUri+'" /></td></tr>\
     <tr class="uriRenameRow"><td colspan="2"><em></em></td></tr>');

  jQuery('#ontologyViewColumRecord').append(
    '<div class="buttonsContainer"><input title="Name" id="renameButton" type="submit" value="Name" onclick="nameUri();" class="renameButton" /></div>');

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function(){});
}

function createClassView()
{
  var uri = currentEntityResultset.subjects[0].uri;

  if(uri == "")
  {
    // ask the user to enter a URI for this new entity

    createIntermediaryUriView();

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
    var prefLabel = currentEntityResultset.subjects[0].getPrefLabelTuple();

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
    
    var altLabels = currentEntityResultset.subjects[0].getAltLabelsTuple();

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
    
    var description = currentEntityResultset.subjects[0].getDescriptionTuple();

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
    jQuery("#ontologyViewColumRecord").append('<table id="annotationsTable">');
    jQuery("#annotationsTable").append(
      '<tr><td id="annotationsTableHeader" colspan="2">Annotations <img id="annotationsHeaderImg" src="'
      + osf_ontologyModuleFullPath + '/imgs/arrow_up.png" title="Collapse panel" /></td></tr>');

    jQuery("#annotationsTableHeader").click(function()
    {
      if(jQuery(".attributeRow").is(':hidden'))
      {
        jQuery(".attributeRow").fadeIn("slow", function()
        {
          jQuery("#annotationsHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
          jQuery("#annotationsHeaderImg").attr("title", "Collapse panel");
        });
      }
      else
      {
        jQuery(".attributeRow").fadeOut("slow", function()
        {
          jQuery("#annotationsHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
          jQuery("#annotationsHeaderImg").attr("title", "Extends panel");
        });
      }
    })

    // Add the preferred label
    var prefLabel = currentEntityResultset.subjects[0].getPrefLabelTuple();

    if(prefLabel.length > 0)
    {
      var inputs = "";

      for(var i = 0; i < prefLabel.length; i++)
      {
        inputs += '<input id="inputPrefLabel_' + i + '" class="attributeInputText" type="text" value="'
          + prefLabel[i].label + '" /><br />';

        jQuery('#inputPrefLabel_' + i).data("attr-uri", prefLabel[i].attr);
      }

      jQuery("#annotationsTable").append(
        '<tr class="attributeRow"><td class="attributeText" valign="top">Preferred Label(s):</td><td valign="top">'
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
    var description = currentEntityResultset.subjects[0].getDescriptionTuple();

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
      '<tr class="attributeRow"><td class="attributeText" valign="top">Description:</td><td valign="top">'
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


  // Display all the structure related attributes & values
  jQuery("#ontologyViewColumRecord").append('<table id="structureTable">');
  jQuery("#structureTable").append(
    '<tr><td id="structureTableHeader" colspan="2">Structure <img id="structureHeaderImg" src="'
    + osf_ontologyModuleFullPath + '/imgs/arrow_up.png" title="Collapse panel" /></td></tr>');

  jQuery("#structureTableHeader").click(function()
  {
    if(jQuery(".structureRow").is(':hidden'))
    {
      jQuery(".structureRow").fadeIn("slow", function()
      {
        jQuery("#structureHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
        jQuery("#structureHeaderImg").attr("title", "Collapse panel");
      });
    }
    else
    {
      jQuery(".structureRow").fadeOut("slow", function()
      {
        jQuery("#structureHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
        jQuery("#structureHeaderImg").attr("title", "Extends panel");
      });
    }
  })

  addAttributeInputAutocomplete("structureTable", "structureSubClass", "structure", "Sub Class of",
    "http://www.w3.org/2000/01/rdf-schema#subClassOf", true, "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#Class", true);
  processedAttributes.push("http://www.w3.org/2000/01/rdf-schema#subClassOf");

  addAttributeInputAutocomplete("structureTable", "structureSuperClass", "structure", "Super Class of",
    "http://umbel.org/umbel#superClassOf", true, "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#Class", true);
  processedAttributes.push("http://umbel.org/umbel#superClassOf");

  addAttributeInputAutocomplete("structureTable", "structureEquivalentClass", "structure", "Equivalent to",
    "http://www.w3.org/2002/07/owl#equivalentClass", true, "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#Class", true);
  processedAttributes.push("http://www.w3.org/2002/07/owl#equivalentClass");

  addAttributeInputAutocomplete("structureTable", "structureDisjointWith", "structure", "Disjoint with",
    "http://www.w3.org/2002/07/owl#disjointWith", true, "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#Class", true);
  processedAttributes.push("http://www.w3.org/2002/07/owl#disjointWith");


  // Display related instances
  jQuery("#ontologyViewColumRecord").append('<table id="instancesTable">');
  jQuery("#instancesTable").append(
    '<tr><td id="instancesTableHeader" colspan="2">Instances <img id="instancesHeaderImg" src="'
    + osf_ontologyModuleFullPath + '/imgs/arrow_down.png" title="Extends panel" /></td></tr>');
  jQuery("#instancesTable").append(
    '<tr class="instanceRow notThisInstanceRow"><td valign="top" colspan="2" id="topInstancesRow"></td></tr>');
  jQuery(".instanceRow").hide();

  jQuery("#instancesTableHeader").click(function()
  {
    if(jQuery('#topInstancesRow').data("initialized") != true)
    {
      jQuery('#topInstancesRow').data("initialized", true);
    }
    else
    {
      if(jQuery(".instanceRow").is(':hidden'))
      {
        jQuery("#topInstancesRow").empty();
        jQuery(".instanceRow").fadeIn("slow", function()
        {
          jQuery("#instancesHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
          jQuery("#instancesHeaderImg").attr("title", "Collapse panel");
        });
      }
      else
      {
        jQuery(".instanceRow").fadeOut("slow", function()
        {
          jQuery("#instancesHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
          jQuery("#instancesHeaderImg").attr("title", "Extends panel");
        });
      }

      return;
    }

    if(jQuery(".instanceRow").is(':hidden'))
    {
      jQuery("#topInstancesRow").empty();

      jQuery("#topInstancesRow").append('<img src="' + osf_ontologyModuleFullPath
        + '/imgs/ajax-loading-bar.gif" width="220" height="19" />');

      jQuery(".instanceRow").fadeIn("slow", function()
      {
        jQuery("#instancesHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
        jQuery("#instancesHeaderImg").attr("title", "Collapse panel");
      });

      jQuery.ajax({
                type: "POST",
                url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                dataType: "json",
                data: {                              
                  ws: network.replace(/\/+$/,"") + "/search/",
                  method: "post",
                  accept: "application/json", 
                  params: "types=" + escape(uri) +
                          "&page=0" +
                          "&items=20" +
                          "&sort="+ escape("preflabel asc") +
                          "&include_aggregates=false"
                },
                "success": function(data)
                {
                  instances = new Resultset(data);

                  jQuery("#topInstancesRow").empty();

                  for(var i = 0; i < instances.subjects.length; i++)
                  {
                    var prefLabel = instances.subjects[i].getPrefLabel();
                    var instanceUri = instances.subjects[i].uri;
                    var datasets = instances.subjects[i].getDatasets();

                    jQuery("#instancesTable").append('<tr class="instanceRow"><td valign="top" colspan="2"><a href="'
                      + structModulesBaseUrl + 'view/?uri=' + escape(instanceUri) + '&dataset=' + escape(datasets[0])
                        + '">' + prefLabel + '</a></td></tr>');
                  }

                  if(instances.subjects.length <= 0)
                  {
                    jQuery("#topInstancesRow").append('No instance records instantiated using this class');
                  }
                  else
                  {
                    if((i % 20 == 0) && (i > 0))
                    {
                      jQuery("#instancesTable").append(
                        '<tr class="instanceRow footerInstanceRow"><td valign="top" colspan="2"><a href="'
                        + structModulesBaseUrl + 'browse/?browse=true&attribute=all&type=' + escape(uri)
                          + '&dataset=all&page=0"><img id="instancesBrowseImg" src="' + osf_ontologyModuleFullPath
                          + '/imgs/world_link.png" />Browse for all records of that type</a></td></tr>');
                      jQuery("#instancesTable").append(
                        '<tr class="instanceRow footerInstanceRow"><td valign="top" colspan="2"><a href="'
                        + structModulesBaseUrl + 'search/?filter_types_1=' + escape(uri)
                          + '&query=&filter=on"><img id="instancesSearchImg" src="' + osf_ontologyModuleFullPath
                          + '/imgs/magnifier.png" />Search for records of that type</a></td></tr>');
                    }
                  }
                },
                "error": function(jqXHR, textStatus, error)
                {
                  jQuery("#topInstancesRow").empty();
                }
              })
    }
    else
    {
      jQuery(".instanceRow").fadeOut("slow", function()
      {
        jQuery("#instancesHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
        jQuery("#instancesHeaderImg").attr("title", "Extends panel");
      });
    }
  })


  // Display all characteristics of this class
  jQuery("#ontologyViewColumRecord").append('<table id="characteristicsTable">');
  jQuery("#characteristicsTable").append(
    '<tr><td id="characteristicsTableHeader" colspan="2">Characteristics <img id="characteristicsHeaderImg" src="'
    + osf_ontologyModuleFullPath + '/imgs/arrow_up.png" title="Collapse panel" /></td></tr>');

  jQuery("#characteristicsTableHeader").click(function()
  {
    if(jQuery(".characteristicRow").is(':hidden'))
    {
      jQuery(".characteristicRow").fadeIn("slow", function()
      {
        jQuery("#characteristicsHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
        jQuery("#characteristicsHeaderImg").attr("title", "Collapse panel");
      });
    }
    else
    {
      jQuery(".characteristicRow").fadeOut("slow", function()
      {
        jQuery("#characteristicsHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
        jQuery("#characteristicsHeaderImg").attr("title", "Extends panel");
      });
    }
  })

  addAttributeInputAutocomplete("characteristicsTable", "characteristics", "characteristics", "Has Characteristic",
    "http://umbel.org/umbel#hasCharacteristic", false, "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/1999/02/22-rdf-syntax-ns#Property",
    false);
  processedAttributes.push("http://umbel.org/umbel#hasCharacteristic");
  
  // Hide the section if it is empty.
  if(!isAdmin && jQuery("#characteristicsTable tr").length <= 1)
  {
    jQuery("#characteristicsTable").hide();
  }

  // Display advanced options
  if(isAdmin)
  {
    jQuery("#ontologyViewColumRecord").append('<table id="advancedTable">');
    jQuery("#advancedTable").append('<tr><td id="advancedTableHeader" colspan="2">Advanced <img id="advancedHeaderImg" src="'
      + osf_ontologyModuleFullPath + '/imgs/arrow_down.png" title="Extend panel" /></td></tr>');

    jQuery("#advancedTableHeader").click(function()
    {
      if(jQuery(".advancedRow").is(':hidden'))
      {
        jQuery(".advancedRow").fadeIn("slow", function()
        {
          jQuery("#advancedHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
          jQuery("#advancedHeaderImg").attr("title", "Collapse panel");
        });
      }
      else
      {
        jQuery(".advancedRow").fadeOut("slow", function()
        {
          jQuery("#advancedHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
          jQuery("#advancedHeaderImg").attr("title", "Extends panel");
        });
      }

      if(jQuery('#templatesSectionTop').data("initialized") != true)
      {
        jQuery("#templatesSectionTop").empty();

        jQuery("#templatesSectionTop").append('<img src="' + osf_ontologyModuleFullPath
          + '/imgs/ajax-loading-bar.gif" width="220" height="19" />');

        jQuery.ajax({
                  type: "POST",
                  url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                  dataType: "json",
                  uri: uri,
                  data: ({
                            ws: network.replace(/\/+$/,"") + "/ontology/read/",
                            method: "post",
                            accept: "application/json", 
                            params: "function=getSuperClasses" +
                                    "&reasoner=" + useReasoner +
                                    "&ontology=" + escape(ontology) +
                                    "&parameters=" + escape("mode=descriptions") + ";" + 
                                                     escape("uri=" + uri) + ";" + 
                                                     escape("direct=0") 
                          }),
                  "success": function(data)
                  {
                    resultset = new Resultset(data);

                    resultset.prefixes = namespacesObj;
                    
                    var templates = [];

                    var uri_record_tpl = 'resource_type__'+resultset.prefixize(this.uri).replace(/[^A-Za-z0-9]/, '_')+'.tpl.php';
                    var uri_search_tpl = 'resource_type__'+resultset.prefixize(this.uri).replace(/[^A-Za-z0-9]/, '_')+'__search.tpl.php';

                    for(var u = 0; u < existingTemplates.length; u++)
                    {
                      if(uri_record_tpl.toLowerCase() == existingTemplates[u].toLowerCase())
                      {
                        templates.push(existingTemplates[u] + '  (record template)');
                      }
                      if(uri_search_tpl.toLowerCase() == existingTemplates[u].toLowerCase())
                      {
                        templates.push(existingTemplates[u] + '  (search template)');
                      }
                    }
                    
                    for(var i = 0; i < resultset.subjects.length; i++)
                    {
                      var superClass_record_tpl = 'resource_type__'+resultset.prefixize(resultset.subjects[i].uri).replace(/[^A-Za-z0-9]/, '_') + '.tpl.php';
                      var superClass_search_tpl = 'resource_type__'+resultset.prefixize(resultset.subjects[i].uri).replace(/[^A-Za-z0-9]/, '_') + '__search.tpl.php';

                      for(var u = 0; u < existingTemplates.length; u++)
                      {
                        if(superClass_record_tpl.toLowerCase() == existingTemplates[u].toLowerCase())
                        {
                          templates.push(existingTemplates[u] + '  (record template)');
                        }
                        if(superClass_search_tpl.toLowerCase() == existingTemplates[u].toLowerCase())
                        {
                          templates.push(existingTemplates[u] + '  (search template)');
                        }
                      }
                    }

                    jQuery("#templatesSectionTop").empty();

                    if(templates.length == 0)
                    {
                      templates = ["resource_type.tpl.php", "resource_type__search.tpl.php"];
                    }

                    for(var i = 0; i < templates.length; i++)
                    {
                      jQuery("#advancedTable").append(
                        '<tr class="advancedRow"><td class="template-item" colspan="2"><img class="templateImg" src="'
                        + osf_ontologyModuleFullPath + '/imgs/color_swatch.png" title="Display template" />'
                        + templates[i] + '</td></tr>');
                    }
                  },
                  "error": function(jqXHR, textStatus, error)
                  {
                    jQuery("#templatesSectionTop").empty();
                    
                    jQuery('#messagesContainer').fadeOut("fast", function()
                    {
                      jQuery('#messagesContainer').empty();
                    });

                    var error = JSON.parse(jqXHR.responseText);

                    var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                    jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg
                      + '</div>').hide().fadeIn("fast");
                  }                  
                })

        jQuery('#templatesSectionTop').data("initialized", true);
      }
    });

    jQuery("#advancedTable").append(
      '<tr class="advancedRow"><td class="advancedSectionHeader" colspan="2">Semantic Components Settings</td></tr>');

    addAttributeInput("advancedTable", "shortLabel", "advanced", "Short label", "http://purl.org/ontology/sco#shortLabel")
    processedAttributes.push("http://purl.org/ontology/sco#shortLabel");

    addAttributeInputAutocomplete("advancedTable", "displayControl", "advanced", "Display component",
      "http://purl.org/ontology/sco#displayControl", false, "http://purl.org/ontology/iron#prefLabel",
      "http://purl.org/ontology/sco#Component", false);
    processedAttributes.push("http://purl.org/ontology/sco#displayControl");

    addAttributeInputAutocomplete("advancedTable", "ignoredBy", "advanced", "Ignored by",
      "http://purl.org/ontology/sco#ignoredBy", false, "http://purl.org/ontology/iron#prefLabel",
      "http://purl.org/ontology/sco#Component", false);
    processedAttributes.push("http://purl.org/ontology/sco#ignoredBy");

    addAttributeInputAutocomplete("advancedTable", "relationBrowserNodeType", "advanced", "Relation browser node type",
      "http://purl.org/ontology/sco#relationBrowserNodeType", false, "http://purl.org/ontology/iron#prefLabel",
      "http://www.w3.org/2002/07/owl#Class", false);
    processedAttributes.push("http://purl.org/ontology/sco#relationBrowserNodeType");

    addAttributeInput("advancedTable", "mapMarkerImageUrl", "advanced", "Map marker image URL",
      "http://purl.org/ontology/sco#mapMarkerImageUrl")
    processedAttributes.push("http://purl.org/ontology/sco#mapMarkerImageUrl");

    // Color picker
    jQuery("#advancedTable").append('<tr class="advancedRow">\
                                     <td class="recordColorText" valign="top">Color:</td>\
                                     <td valign="top">\
                                       <div id="colorSelector">\
                                         <div style="background-color: #0000ff"></div>\
                                       </div>\
                                     </td>\
                                   </tr>');

    // Set the default color to null
    jQuery('#colorSelector').data("attr-uri", "http://purl.org/ontology/sco#color");
    jQuery('#colorSelector').data("value-type", "literal");
    jQuery('#colorSelector').data("value", null);
    
    var defaultColor = '#0000ff';
    
    var values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#color");
    
    if(values.length > 0)
    {
      defaultColor = values[0];
    }
                                   
    jQuery('#colorSelector').ColorPicker({ color: defaultColor, 
                                      onShow: function (colpkr) { jQuery(colpkr).fadeIn(500); return false; }, 
                                      onHide: function (colpkr) { jQuery(colpkr).fadeOut(500); return false; }, 
                                      onChange: function (hsb, hex, rgb) { 
                                        jQuery('#colorSelector div').css('backgroundColor', '#' + hex);
                                        jQuery('#colorSelector').data("value", '#' + hex); 
                                      }
                                    });
                                    
    jQuery('#colorSelector div').css('backgroundColor', defaultColor);                                    

    processedAttributes.push("http://purl.org/ontology/sco#color");

  //jQuery("#advancedTable").append('<tr class="advancedRow"><td class="advancedSectionHeader" colspan="2">UMBEL Settings</td></tr>');
    jQuery("#advancedTable").append(
      '<tr class="advancedRow"><td class="advancedSectionHeader" colspan="2">Display Settings (in selection order)</td></tr>');
    jQuery("#advancedTable").append('<tr class="advancedRow"><td id="templatesSectionTop" colspan="2"></td></tr>');

    jQuery('#templatesSection').data("initialized", false);

    jQuery(".advancedRow").hide();      
  }     

  var sectionHeaderDisplayed = false;

  if(typeof (currentEntityResultset.subjects[0]) == 'object' && 'predicate' in currentEntityResultset.subjects[0])
  {
    var index = 0;

    for(var i = 0; i < currentEntityResultset.subjects[0].predicate.length; i++)
    {
      for(var predicate in currentEntityResultset.subjects[0].predicate[i])
      {
        if(currentEntityResultset.unprefixize(predicate) == "http://umbel.org/umbel#superClassOf")
        {
          continue;
        }

        var attrUri = currentEntityResultset.unprefixize(predicate);

        if(processedAttributes.indexOf(attrUri) == -1)
        {
          if(!sectionHeaderDisplayed)
          {
            // Display all the attributes/values that haven't be handled in any other section above.
            jQuery("#ontologyViewColumRecord").append('<table id="othersTable">');
            jQuery("#othersTable").append('<tr><td id="othersTableHeader" colspan="2">Others <img id="othersHeaderImg" src="'
              + osf_ontologyModuleFullPath + '/imgs/arrow_up.png" title="Collapse panel" /></td></tr>');

            jQuery("#othersTableHeader").click(function()
            {
              if(jQuery(".otherRow").is(':hidden'))
              {
                jQuery(".otherRow").fadeIn("slow", function()
                {
                  jQuery("#othersHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_up.png');
                  jQuery("#othersHeaderImg").attr("title", "Collapse panel");
                });
              }
              else
              {
                jQuery(".otherRow").fadeOut("slow", function()
                {
                  jQuery("#othersHeaderImg").attr("src", osf_ontologyModuleFullPath + '/imgs/arrow_down.png');
                  jQuery("#othersHeaderImg").attr("title", "Extends panel");
                });
              }
            })

            sectionHeaderDisplayed = true;
          }

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

          var values = currentEntityResultset.subjects[0].getPredicateValues(attrUri);

          var inputs = "";

          for(var ii = 0; ii < values.length; ii++)
          {
            var value = "";

            if(typeof (values[ii]) == 'object' && 'uri' in values[ii])
            {
              value = values[ii].uri;
            }
            else
            {
              value = values[ii];
            }

            if(isAdmin)
            {
              inputs += '<input type="text" id="inputOthers_' + index + '_' + ii
                + '" class="attributeInputTextArea" value="' + value + '" /><br />';
            }
            else
            {
              inputs += '<div id="inputOthers_' + index + '_' + ii
                     + '" class="attributeInputTextArea">' + value + '</div>'
            }            
          }

          jQuery("#othersTable").append('<tr class="otherRow"><td class="attributeText" valign="top" title="' + attrUri
            + '">' + attrLabel + ':</td><td valign="top">' + inputs + '</td></tr>');

          // Set the data associated with all created elements.
          if(isAdmin)
          {
            for(var ii = 0; ii < values.length; ii++)
            {
              if(typeof (values[ii]) == 'object' && 'uri' in values[ii])
              {
                jQuery('#inputOthers_' + index + '_' + ii).data("value-type", "uri");
              }
              else
              {
                jQuery('#inputOthers_' + index + '_' + ii).data("value-type", "literal");
              }

              jQuery('#inputOthers_' + index + '_' + ii).data("attr-uri", attrUri);
              processedAttributes.push(attrUri);
            }
          }

          index++;
        }
      }
    }
  }

  if (Drupal.settings.osf_ontology.browse) {
    jQuery('#ontologyViewColumRecord').append('<div class="buttonsContainer"><input title="Add this term to the current node" id="saveButton" type="submit" value="Add to node" onclick="addToNode();" class="saveButton addToNodeButton" style="display: inline;" /></div>');
  } else if(isAdmin) {
    jQuery('#ontologyViewColumRecord').append('<div class="buttonsContainer"><input title="Save changes" id="saveButton" type="submit" value="Save" onclick="save();" class="saveButton" /></div>');
  }

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function(){});
}

function nameUri()
{
  // Make sure the user did put something for for URI
  if(jQuery("#uriRenameInputText").val() == baseOntologyUri)
  {
     alert("You have to specify a unique identifier, at the end of the Ontology's URI, in order to create the URI of this new resource.");
  }
  else
  {
    jQuery("#renameButton").hide();

    currentEntityResultset.subjects[0].uri = jQuery("#uriRenameInputText").val();

    jQuery('#messagesContainer').fadeOut("fast", function()
    {
      jQuery('#messagesContainer').empty();
    });

    reloadTree = true;

    switch(currentView)
    {
      case "classes":
        createClassView();
        break;

      case "properties":
        createPropertyView();
        break;
        
      case "individuals":

        var rset = new Resultset({
              prefixes: [],
              resultset: {
                        subject: [
                          {
                                    uri: jQuery("#uriRenameInputText").val(),
                                    type: "",
                                    predicate: []
                                  }
                        ]
                      }
            });    
      
        createNamedIndividualView(rset);
        break;
    }
  }
}

function renameUri(oldUri)
{
  jQuery("#renameButton").hide();

  if(jQuery("#renameButtonAjax").length <= 0)
  {
    jQuery("#ontologyViewColumRecord").append('<div class="buttonsContainer"><img id="renameButtonAjax" src="'
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
            "data": {
                      ws: network.replace(/\/+$/,"") + "/ontology/update/",
                      method: "post",
                      accept: "application/json",
                      params: "ontology=" + escape(ontology) + "&function=updateEntityUri&parameters="
                        + escape("oldUri=" + escape(oldUri)) + ";"
                        + escape("newUri=" + escape(jQuery("#uriRenameInputText").val())) + ";"
                        + escape("advancedIndexation=true")
                    },
            "success": function(data)
            {
              classUriRenameSuccess();
            },
            "error": function(jqXHR, textStatus, error)
            {
              // The Ontology: Update web service does return an empty body, and not an empty JSON object.
              // This means that we have to rely on the HTTP status to know if the query is a success or 
              // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
              // string which returns a parse error.
              if(jqXHR.status == 200)
              {
                classUriRenameSuccess();
              }
              else
              {              
                jQuery('#messagesContainer').fadeOut("fast", function()
                {
                  jQuery('#messagesContainer').empty();

                  jQuery("#renameButtonAjax").hide();
                  jQuery("#renameButton").show();
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
          })
}

function addToNode()
{
  container = jQuery('.open-popup').parents('fieldset');
  jQuery('.open-popup').removeClass('open-popup');
  //termName = jQuery("#inputPrefLabel_0").val();
  termName = jQuery('.jstree-clicked').parent().attr('title');
  jQuery('#classesTree').jstree('destroy');
  jQuery('.modal-header .close').click();
  container.find('input.form-autocomplete').val(termName).trigger("blur");
}

jQuery('#modalContent .close').live('mousedown', function() {
  jQuery('#classesTree').jstree('destroy');
});

function save()
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

  // Get first Type
  var type = currentEntityResultset.subjects[0].type;

  // @TODO: put all other types into the "predicates" var

  // Get prefLabel(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#inputPrefLabel_" + i).length > 0 && jQuery("#inputPrefLabel_" + i).val().length > 0)
    {
      var prefLabelObj = {
              };

      prefLabelObj[jQuery("#inputPrefLabel_" + i).data("attr-uri")] = jQuery("#inputPrefLabel_" + i).val();
      predicates.push(prefLabelObj);
    }
    else
    {
      break;
    }
  }

  // Get altLabels(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#inputAltLabel_" + i).length > 0 && jQuery("#inputAltLabel_" + i).val().length > 0)
    {
      var altLabelObj = {
              };

      altLabelObj[jQuery("#inputAltLabel_" + i).data("attr-uri")] = jQuery("#inputAltLabel_" + i).val();
      predicates.push(altLabelObj);
    }
    else
    {
      break;
    }
  }

  // Get descriptions(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#inputDescription_" + i).length > 0 && jQuery("#inputDescription_" + i).val().length > 0)
    {
      var descriptionObj = {
              };

      descriptionObj[jQuery("#inputDescription_" + i).data("attr-uri")] = jQuery("#inputDescription_" + i).val();
      predicates.push(descriptionObj);
    }
    else
    {
      break;
    }
  }

  // Get Sub Classes relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureSubClass_" + i).length > 0 && jQuery("#structureSubClass_" + i).val().length > 0)
    {
      var subClassObj = {
              };

      subClassObj[jQuery('#structureSubClass_'
        + i).data("attr-uri")] = {
                uri: jQuery('#structureSubClass_' + i).data("value-uri")
              };

      predicates.push(subClassObj);
    }
    else
    {
      break;
    }
  }

  // Get Disjoint with relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureDisjointWith_" + i).length > 0 && jQuery("#structureDisjointWith_" + i).val().length > 0)
    {
      var disjointWithObj = {
              };

      disjointWithObj[jQuery('#structureDisjointWith_'
        + i).data("attr-uri")] = {
                uri: jQuery('#structureDisjointWith_' + i).data("value-uri")
              };

      predicates.push(disjointWithObj);
    }
    else
    {
      break;
    }
  }

  // Get Equivalent class with relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureEquivalentClass_" + i).length > 0 && jQuery("#structureEquivalentClass_" + i).val().length > 0)
    {
      var equivalentClassObj = {
              };

      equivalentClassObj[jQuery('#structureEquivalentClass_'
        + i).data("attr-uri")] = {
                uri: jQuery('#structureEquivalentClass_' + i).data("value-uri")
              };

      predicates.push(equivalentClassObj);
    }
    else
    {
      break;
    }
  }

  // Get hasCharacteristic relationships
  for(i = 0; i < 512; i++)
  {
    if(jQuery("#characteristics_" + i).length > 0 && jQuery("#characteristics_" + i).val().length > 0)
    {
      var hasCharacteristicClassObj = {
              };

      hasCharacteristicClassObj[jQuery('#characteristics_'
        + i).data("attr-uri")] = {
                uri: jQuery('#characteristics_' + i).data("value-uri")
              };

      predicates.push(hasCharacteristicClassObj);
    }
    else
    {
      break;
    }
  }

  // Get displayControl relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#displayControl_" + i).length > 0 && jQuery("#displayControl_" + i).val().length > 0)
    {
      var displayControlClassObj = {
              };

      displayControlClassObj[jQuery('#displayControl_'
        + i).data("attr-uri")] = {
                uri: jQuery('#displayControl_' + i).data("value-uri")
              };

      predicates.push(displayControlClassObj);
    }
    else
    {
      break;
    }
  }
  
  // Get ignoredBy relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#ignoredBy_" + i).length > 0 && jQuery("#ignoredBy_" + i).val().length > 0)
    {
      var ignoredByClassObj = {
              };

      ignoredByClassObj[jQuery('#ignoredBy_'
        + i).data("attr-uri")] = {
                uri: jQuery('#ignoredBy_' + i).data("value-uri")
              };

      predicates.push(ignoredByClassObj);
    }
    else
    {
      break;
    }
  }  

  // Get relationBrowserNodeType relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#relationBrowserNodeType_" + i).length > 0 && jQuery("#relationBrowserNodeType_" + i).val().length > 0)
    {
      var relationBrowserNodeTypeClassObj = {
              };

      relationBrowserNodeTypeClassObj[jQuery('#relationBrowserNodeType_'
        + i).data("attr-uri")] = {
                uri: jQuery('#relationBrowserNodeType_' + i).data("value-uri")
              };

      predicates.push(relationBrowserNodeTypeClassObj);
    }
    else
    {
      break;
    }
  }


  // Get shortLabel relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#shortLabel_" + i).length > 0 && jQuery("#shortLabel_" + i).val().length > 0)
    {
      var shortLabelObj = {
              };

      shortLabelObj[jQuery('#shortLabel_' + i).data("attr-uri")] = jQuery('#shortLabel_' + i).val();
      predicates.push(shortLabelObj);
    }
    else
    {
      break;
    }
  }

  // Get mapMarkerImageUrl relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#mapMarkerImageUrl_" + i).length > 0 && jQuery("#mapMarkerImageUrl_" + i).val().length > 0)
    {
      var mapMarkerImageUrlObj = {
              };

      mapMarkerImageUrlObj[jQuery('#mapMarkerImageUrl_' + i).data("attr-uri")] = jQuery('#mapMarkerImageUrl_' + i).val();
      predicates.push(mapMarkerImageUrlObj);
    }
    else
    {
      break;
    }
  }

  // Get sco:color relationships
  if(jQuery('#colorSelector').data("value") != null)
  {
    var recordColorObj = {
            };

    recordColorObj[jQuery('#colorSelector').data("attr-uri")] = jQuery('#colorSelector').data("value");
    predicates.push(recordColorObj);
  }
    
  // Get all other properties(s)
  for(i = 0; i < 256; i++)
  {
    for(ii = 0; ii < 256; ii++)
    {
      if(jQuery('#inputOthers_' + i + '_' + ii).length > 0 && jQuery('#inputOthers_' + i + '_' + ii).val().length > 0)
      {
        var otherObj = {
                };

        if(jQuery('#inputOthers_' + i + '_' + ii).data("value-type") == "literal")
        {
          otherObj[jQuery('#inputOthers_' + i + '_' + ii).data("attr-uri")] = jQuery('#inputOthers_' + i + '_' + ii).val();
        }
        else if(jQuery('#inputOthers_' + i + '_' + ii).data("value-type") == "uri")
        {
          otherObj[jQuery('#inputOthers_' + i + '_'
            + ii).data("attr-uri")] = {
                    uri: jQuery('#inputOthers_' + i + '_' + ii).val()
                  };
        }

        predicates.push(otherObj);
      }
      else
      {
        break;
      }
    }

    if(jQuery('#inputOthers_' + (i + 1) + '_0').length <= 0)
    {
      break;
    }
  }

  rset = new Resultset({
            prefixes: currentEntityResultset.prefixes,
            resultset: {
                      subject: [
                        {
                                  uri: currentEntityResultset.subjects[0].uri,
                                  type: type,
                                  predicate: []
                                }
                      ]
                    }
          });

  rset.subjects[0].predicate = predicates;

  // Get Super Classes relationship(s)
  // Note: the superClassOf relationship are converted into subClassOf relationships by
  //       updating these sub-classes records with the subClassOf relationship to this
  //       super class.
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureSuperClass_" + i).length > 0 && jQuery("#structureSuperClass_" + i).val().length > 0)
    {
      var subClassResultset = jQuery.ajax({
                type: "POST",
                url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                dataType: "json",
                data: ({
                          ws: network.replace(/\/+$/,"") + "/ontology/read/",
                          method: "post",
                          accept: "application/json", 
                          params: "function=getClass" +
                                  "&reasoner=" + useReasoner +
                                  "&ontology=" + escape(ontology) +
                                  "&parameters=" + escape("uri=" + jQuery('#structureSuperClass_' + i).data("value-uri")) 
                        }),
                async: false
              }).responseText;

      var jsonResponseText = JSON.parse(subClassResultset);

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

      subClassResultset = new Resultset(jsonResponseText);

      // Remove the superClassOf relationship since we don't want to persist it into the ontologies store
      subClassResultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superClassOf");

      // Make sure to remove the attribute/value that we are about to introduce.
      // That way we make sure not to duplicate the relationship.
      subClassResultset.subjects[0].removePredicateValue("http://www.w3.org/2000/01/rdf-schema#subClassOf",
        currentEntityResultset.subjects[0].uri);

      // Add that class as a sub-class-of the target class being manipulated
      subClassResultset.subjects[0].addAttributeValue("http://www.w3.org/2000/01/rdf-schema#subClassOf", {
                uri: currentEntityResultset.subjects[0].uri
              });

      rset.subjects.push(subClassResultset.subjects[0]);
    }
    else
    {
      break;
    }
  }

  // Do the same as above, but for super-class-of relationships that have been deleted for this target class.
  for(i = 0; i < removedSuperEntityOf.length; i++)
  {
    var subClassResultset = jQuery.ajax({
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              dataType: "json",
              data: ({
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=getClass" +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
                                "&parameters=" + escape("uri=" + removedSuperEntityOf[i])
                      }),
              async: false
            }).responseText;

    var jsonResponseText = JSON.parse(subClassResultset);

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

    subClassResultset = new Resultset(jsonResponseText);

    // Remove the superClassOf relationship since we don't want to persist it into the ontologies store
    subClassResultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superClassOf");

    // Remove the sub-class-of the target class being manipulated
    subClassResultset.subjects[0].removePredicateValue("http://www.w3.org/2000/01/rdf-schema#subClassOf",
      currentEntityResultset.subjects[0].uri);

    rset.subjects.push(subClassResultset.subjects[0]);
  }

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
                        params: "function=getClass" +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
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

  // Possibly rename the node in the tree
  var contents = jQuery.jstree._focused().get_selected().children("a").contents();

  if(contents.length > 0)
  {
    contents[contents.length - 1].nodeValue = rset.subjects[0].getPrefLabel();
  }

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
              classSaveSuccess();
            },
            "error": function(jqXHR, textStatus, error)
            {
              // The Ontology: Update web service does return an empty body, and not an empty JSON object.
              // This means that we have to rely on the HTTP status to know if the query is a success or 
              // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
              // string which returns a parse error.
              if(jqXHR.status == 200)
              {
                classSaveSuccess();
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

function getClassesTreeOrderedPath(resultset, targetSuperClass)
{
  var path = [targetSuperClass];

  for(var i = 0; i < resultset.subjects.length; i++)
  {
    var superClasses = resultset.subjects[i].getPredicateValues("http://www.w3.org/2000/01/rdf-schema#subClassOf");

    for(var ii = 0; ii < superClasses.length; ii++)
    {
      if(superClasses[ii].uri == targetSuperClass)
      {
        var classes = getClassesTreeOrderedPath(resultset, resultset.subjects[i].uri);

        for(var iii = 0; iii < classes.length; iii++)
        {
          path.push(classes[iii]);
        }
      }
    }
  }

  return(path);
}

function submitViewSearchEnter(field, e)
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
    viewSearch();

    return false;
  }
  else
  {
    return true;
  }
}

function viewSearch()
{
  switch(currentView)
  {
    case "classes":
      jQuery("#classesTree").jstree("search",/*"structureSubClass",*/ jQuery("#searchBox").val(), false);
      break;

    case "properties":
      jQuery("#propertiesTree").jstree("search",/*"structureSubProperty",*/ jQuery("#searchBox").val(), false);
      break;
  }
}

function addAttributeInputAutocomplete(containerID, attributePrefix, rowPrefix, attributeLabel, targetPredicate,
  forceReloadTree, searchFilterAttributes, searchFilterTypes, jsDrop)
{
  var attributes = currentEntityResultset.subjects[0].getPredicateValues(targetPredicate);

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
            + (jsDrop ? "jstree-drop" : "") + '" type="text" value="" />';
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
          + attributePrefix + 'InputText ' + (jsDrop ? "jstree-drop" : "") + '" type="text" value="' + val + '" />';
        inputs += '<img id="' + attributePrefix + '_' + i + '_img" class="' + attributePrefix
          + 'AddImg structureAddImg" src="' + osf_ontologyModuleFullPath
          + '/imgs/bullet_delete.png" title="remove" /><br />';
      }
      else
      {
        if(jsDrop == true &&
           targetPredicate != "http://www.w3.org/2000/01/rdf-schema#subPropertyOf")
        {
          inputs += '<div id="' + attributePrefix + '_' + i + '" class="' + attributePrefix
                  + 'InputText"><a class="jsTreeLinksReadingMode" onclick="jQuery(\'#classesTree\').jstree(\'search\', \''+val.replace(/^\s+/,"").replace(/\s+$/,"")+'\');">' + val + '</a></div>'                  
        }
        else
        {
          inputs += '<div id="' + attributePrefix + '_' + i + '" class="' + attributePrefix
                  + 'InputText">' + val + '</div>'
        }
                
        hasValues = true;
      } 
    }
  }
  else
  {
    if(isAdmin)
    {
      inputs += '<input id="' + attributePrefix + '_0" class="' + attributePrefix + 'InputText '
        + (jsDrop ? "jstree-drop" : "") + '" type="text" value="" />';
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
        jQuery('#' + attributePrefix + '_' + i).data("force-reload", forceReloadTree);
        jQuery('#' + attributePrefix + '_' + i + '_img').click(function()
        {
          var id = jQuery(this).attr("id").replace("_img", "");

          // Prevent re-adding of removed super classes
          if(removedSuperEntityOf.indexOf(id) != -1)
          {
            removedSuperEntityOf.slice(removedSuperEntityOf.indexOf(id), 1);
          }

          addInputAutocomplete(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"), true,
            jQuery("#" + id).data("search-filter-attributes"), jQuery("#" + id).data("search-filter-types"));

          reloadTree = jQuery("#" + id).data("force-reload");
        });
      }
      else
      {
        jQuery('#' + attributePrefix + '_' + i).data("value-uri", attributes[i].uri);
        jQuery('#' + attributePrefix + '_' + i).data("attr-prefix", attributePrefix);
        jQuery('#' + attributePrefix + '_' + i).data("force-reload", forceReloadTree);
        jQuery('#' + attributePrefix + '_' + i + '_img').click(function()
        {
          var id = jQuery(this).attr("id").replace("_img", "");

          if(jQuery("#" + id).data("attr-prefix").search("SuperClass") != -1)
          {
            removedSuperEntityOf.push(jQuery("#" + id).data("value-uri"));
          }

          removeInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));

          reloadTree = jQuery("#" + id).data("force-reload");
        });
      }

      jQuery('#' + attributePrefix + '_' + i).data("attr-uri", targetPredicate);
      jQuery('#' + attributePrefix + '_' + i).data("value-type", "uri");
      jQuery('#' + attributePrefix + '_' + i).data("element-id", i);
      jQuery('#' + attributePrefix + '_' + i).data("search-filter-attributes", searchFilterAttributes);
      jQuery('#' + attributePrefix + '_' + i).data("search-filter-types", searchFilterTypes);
      jQuery('#' + attributePrefix + '_' + i).data("jsdrop", jsDrop);

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

                    addInputAutocomplete(me.data("attr-prefix"), me.data("element-id"), me.data("jsdrop"),
                      me.data("search-filter-attributes"), me.data("search-filter-types"));

                    if(me.data("force-reload"))
                    {
                      reloadTree = true;
                    }
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

function toggleWaitingIcon(imgID)
{
  if(jQuery("#" + imgID).attr("src").search("ajax-loader-icon.gif") != -1)
  {
    jQuery("#" + imgID).attr("src", jQuery("#" + imgID).data("prevImg"))
  }
  else
  {
    jQuery("#" + imgID).data("prevImg", jQuery("#" + imgID).attr("src"));
    jQuery("#" + imgID).attr("src", osf_ontologyModuleFullPath + '/imgs/ajax-loader-icon.gif');
  }
}

function removeInput(id, num)
{
  // Delete everything related to this entry
  jQuery('#' + id + '_' + num).remove();                 // The input
  jQuery('#' + id + '_' + num + '_img').next().remove(); // The last <br />
  jQuery('#' + id + '_' + num + '_img').remove();        // The image button.

  // Now re-number all the inputs of the same groupe.
  for(i = (num + 1); i < 256; i++)
  {
    if(jQuery("#" + id + "_" + i).length > 0)
    {
      // Re-number all the related items to this entry
      jQuery('#' + id + '_' + i).data("element-id", (i - 1));
      jQuery('#' + id + '_' + i + '_img').attr("id", id + '_' + (i - 1) + '_img'); // The image button.
      jQuery('#' + id + '_' + i).attr("id", id + '_' + (i - 1));                   // The input
    }
    else
    {
      break;
    }
  }
}

function addInputAutocomplete(id, num, jsDrop, searchFilterAttributes, searchFilterTypes)
{
  var input = "";

  input += '<input title="" id="' + id + '_' + (num + 1) + '" class="' + id + 'InputText '
    + (jsDrop ? "jstree-drop" : "") + '" type="text" value="" />';

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
  jQuery('#' + id + '_' + (num + 1)).data("jsdrop", jsDrop);

  if(jsDrop == false)
  {
    jQuery('#' + id + '_' + (num + 1) + '_img').click(function()
    {
      var id = jQuery(this).attr("id").replace("_img", "");
      addInputAutocomplete(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"), false,
        jQuery("#" + id).data("search-filter-attributes"), jQuery("#" + id).data("search-filter-types"));

      // Prevent re-adding of removed super classes
      if(removedSuperEntityOf.indexOf(id) != -1)
      {
        removedSuperEntityOf.slice(removedSuperEntityOf.indexOf(id), 1);
      }

      reloadTree = jQuery("#" + id).data("force-reload");
    });
  }
  else
  {
    jQuery('#' + id + '_' + (num + 1) + '_img').click(function()
    {
      var id = jQuery(this).attr("id").replace("_img", "");
      addInputAutocomplete(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"), true,
        jQuery("#" + id).data("search-filter-attributes"), jQuery("#" + id).data("search-filter-types"));

      // Prevent re-adding of removed super classes
      if(removedSuperEntityOf.indexOf(id) != -1)
      {
        removedSuperEntityOf.slice(removedSuperEntityOf.indexOf(id), 1);
      }

      reloadTree = jQuery("#" + id).data("force-reload");
    });
  }

  jQuery('#' + id + '_' + num + '_img').attr("src", osf_ontologyModuleFullPath + '/imgs/bullet_delete.png');
  jQuery('#' + id + '_' + num + '_img').attr("title", 'remove');
  jQuery('#' + id + '_' + num + '_img').unbind("click");

  jQuery('#' + id + '_' + num + '_img').click(function()
  {
    var id = jQuery(this).attr("id").replace("_img", "");
    removeInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));

    if(jQuery("#" + id).data("attr-prefix").search("SuperClass") != -1)
    {
      removedSuperEntityOf.push(jQuery("#" + id).data("value-uri"));
    }

    reloadTree = jQuery("#" + id).data("force-reload");
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

                addInputAutocomplete(me.data("attr-prefix"), me.data("element-id"), me.data("jsdrop"),
                  me.data("search-filter-attributes"), me.data("search-filter-types"));

                if(me.data("force-reload"))
                {
                  reloadTree = true;
                }
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

function addAttributeInput(containerID, attributePrefix, rowPrefix, attributeLabel, targetPredicate)
{
  var attributes = currentEntityResultset.subjects[0].getPredicateValues(targetPredicate);

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
        inputs += '<input title="' + attributes[i].uri + '" id="' + attributePrefix + '_' + i + '" class="'
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

function addInput(id, num)
{
  var input = "";

  input += '<input title="" id="' + id + '_' + (num + 1) + '" class="' + id + 'InputText" type="text" value="" />';

  input += '<img id="' + id + '_' + (num + 1) + '_img" class="' + id + 'AddImg structureAddImg" src="'
    + osf_ontologyModuleFullPath + '/imgs/bullet_add.png" title="add" /><br />';

  jQuery('#' + id + '_' + num).parent().append(input);

  jQuery('#' + id + '_' + (num + 1)).data("value-uri", "");
  jQuery('#' + id + '_' + (num + 1)).data("attr-uri", jQuery('#' + id + '_' + num).data("attr-uri"));
  jQuery('#' + id + '_' + (num + 1)).data("value-type", "literal");
  jQuery('#' + id + '_' + (num + 1)).data("element-id", (num + 1));
  jQuery('#' + id + '_' + (num + 1)).data("attr-prefix", id);

  jQuery('#' + id + '_' + (num + 1) + '_img').click(function()
  {
    var id = jQuery(this).attr("id").replace("_img", "");
    addInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));
  });

  jQuery('#' + id + '_' + num + '_img').attr("src", osf_ontologyModuleFullPath + '/imgs/bullet_delete.png');
  jQuery('#' + id + '_' + num + '_img').attr("title", 'remove');
  jQuery('#' + id + '_' + num + '_img').unbind("click");

  jQuery('#' + id + '_' + num + '_img').click(function()
  {
    var id = jQuery(this).attr("id").replace("_img", "");
    removeInput(jQuery("#" + id).data("attr-prefix"), jQuery("#" + id).data("element-id"));
  });
}

function toggleClassesView()
{
  currentView = "classes";

  removedSuperEntityOf = [];

  //jQuery("#classesTree").jstree("select_node", "#"+lastSelectedClassNode);
  jQuery("#" + lastSelectedClassNode).children("a").click();

  searchAc.options.params = {
            datasets: ontology,
            attributes: "http://purl.org/ontology/iron#prefLabel",
            page: 0,
            items: 20,
            include_aggregates: "false",
            types: "http://www.w3.org/2002/07/owl#Class"
          };

  jQuery('#searchBox').val("");

  jQuery("#classesTab").removeClass("classesTabUnselected");
  jQuery("#classesTab").addClass("classesTabSelected");
  jQuery("#propertiesTab").removeClass("propertiesTabSelected");
  jQuery("#propertiesTab").addClass("propertiesTabUnselected");
  jQuery("#individualsTab").removeClass("individualsTabSelected");
  jQuery("#individualsTab").addClass("individualsTabUnselected");

  if(propertiesTreeCreated)
  {
    jQuery("#propertiesTree").hide();
  }
  
  if(individualsListCreated)
  {
    jQuery("#individualsList").hide();
  } 

  jQuery("#classesTree").show();
  
  if(jQuery(".searchBoxContainer").length > 0)
  {
    jQuery(".searchBoxContainer").show();
  }
}

function togglePropertiesView()
{
  currentView = "properties";

  removedSuperEntityOf = [];

  //jQuery("#propertiesTree").jstree("select_node", "#"+lastSelectedPropertyNode);
  jQuery("#" + lastSelectedPropertyNode).children("a").click();

  searchAc.options.params = {
            datasets: ontology,
            attributes: "http://purl.org/ontology/iron#prefLabel",
            page: 0,
            items: 20,
            include_aggregates: "false",
            types:
              "http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/2002/07/owl#AnnotationProperty"
          };

  jQuery('#searchBox').val("");

  jQuery("#classesTab").removeClass("classesTabSelected");
  jQuery("#classesTab").addClass("classesTabUnselected");
  jQuery("#propertiesTab").removeClass("propertiesTabUnselected");
  jQuery("#propertiesTab").addClass("propertiesTabSelected");
  jQuery("#individualsTab").removeClass("individualsTabSelected");
  jQuery("#individualsTab").addClass("individualsTabUnselected");

  jQuery("#classesTree").hide();

  if(individualsListCreated)
  {
    jQuery("#individualsList").hide();
  }
  
  jQuery("#propertiesTree").show();
  
  if(!propertiesTreeCreated)
  {
    createPropertiesTree("http://www.w3.org/2002/07/owl#TopProperty");
  }
  
  if(jQuery(".searchBoxContainer").length > 0)
  {
    jQuery(".searchBoxContainer").show();
  }  
}

function toggleIndividualsView()
{
  currentView = "individuals";
   
  jQuery("#classesTab").removeClass("classesTabSelected");
  jQuery("#classesTab").addClass("classesTabUnselected");
  jQuery("#propertiesTab").removeClass("propertiesTabSelected");
  jQuery("#propertiesTab").addClass("propertiesTabUnselected");
  jQuery("#individualsTab").addClass("individualsTabSelected");
  jQuery("#individualsTab").removeClass("individualsTabUnselected");  
  
  jQuery("#classesTree").hide();
  jQuery("#propertiesTree").hide();
  
  jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function(){});
  
  if(individualsListCreated)
  {
    selectIndividual(previouslySelectedNamedEntity);
    jQuery("#individualsList").show();
  }
  else
  {
    createIndividualsList();
  }  
  
  if(jQuery(".searchBoxContainer").length > 0)
  {
    jQuery(".searchBoxContainer").hide();
  }  
}

function toggleReasoner()
{
  if(jQuery("#reasonerButtonImg").attr("src").indexOf("cog_add") != -1)
  {
    // Enable the reasoner
    var imgUrl = jQuery("#reasonerButtonImg").attr("src");
    jQuery("#reasonerButtonImg").attr("src", imgUrl.replace("cog_add", "cog_delete"));

    var imgUrl = jQuery("#reasonerBeaconImg").attr("src");
    jQuery("#reasonerBeaconImg").attr("src", imgUrl.replace("bullet_red", "bullet_green"));

    jQuery("#reasonerButtonImg").attr("title", "Disable Reasoner");
    jQuery("#reasonerBeaconImg").attr("title", "Reasoner currently enabled");
    
    useReasoner = "true";
    
    // force tree refresh
    if(jQuery("#httpwwww3org200207owlThing").length > 0)
    {
      jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlThing");
    }
    
    if(jQuery("#httpwwww3org200207owlTopProperty").length > 0)
    {
      jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlTopProperty");
    }
    
  }
  else
  {
    // Disable the reasoner
    var imgUrl = jQuery("#reasonerButtonImg").attr("src");
    jQuery("#reasonerButtonImg").attr("src", imgUrl.replace("cog_delete", "cog_add"));
    
    var imgUrl = jQuery("#reasonerBeaconImg").attr("src");
    jQuery("#reasonerBeaconImg").attr("src", imgUrl.replace("bullet_green", "bullet_red"));    
    
    jQuery("#reasonerButtonImg").attr("title", "Enable Reasoner");
    jQuery("#reasonerBeaconImg").attr("title", "Reasoner currently disabled");
    
    useReasoner = "false";
    
    // force tree refresh
    if(jQuery("#httpwwww3org200207owlThing").length > 0)
    {
      jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlThing");
    }
    
    if(jQuery("#httpwwww3org200207owlTopProperty").length > 0)
    {
      jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlTopProperty");
    }    
  }
}

function classMoveSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#messagesContainer').append(
    '<div class="successBox">Class successfully moved</div>').hide().fadeIn("fast");
}

function classDeleteSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlThing");

  jQuery('#messagesContainer').append(
    '<div class="successBox">Class successfully deleted</div>').hide().fadeIn("fast");
}

function classRenameSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#messagesContainer').append(
    '<div class="successBox">Class successfully renamed</div>').hide().fadeIn("fast");
}


function classUriRenameSuccess()
{
  jQuery('#messagesContainer').append('<div class="successBox">URI successfully redefined</div>').hide().fadeIn(
  "fast");

  jQuery("#renameButtonAjax").hide();
  jQuery("#renameButton").show();

  jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlThing");
  reloadTree = false;
}

function classSaveSuccess()
{
  jQuery('#messagesContainer').append('<div class="successBox">Class successfully saved</div>').hide().fadeIn(
  "fast");

  jQuery("#saveButtonAjax").hide();
  jQuery("#saveButton").show();

  removedSuperEntityOf = [];

  if(reloadTree)
  {
    jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlThing");
    reloadTree = false;
  }
}
