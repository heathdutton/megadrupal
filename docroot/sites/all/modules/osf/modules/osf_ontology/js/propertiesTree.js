var propertiesTreeNodeBinder = [];

propertiesTreeNodeBinder["httpwwww3org200207owltopDataProperty"] = "http://www.w3.org/2002/07/owl#topDataProperty";
propertiesTreeNodeBinder["httpwwww3org200207owltopObjectProperty"] = "http://www.w3.org/2002/07/owl#topObjectProperty";

function createPropertiesTree(uri)
{
  var jstreeStructure = {
    core: {
              initially_open: [uri.replace(/[^a-zA-Z0-9]+/g, '')],
              strings: {
                        loading: "Loading ...",
                        new_node: "New property"
                      },
            },
    "json_data": {
              "data": [
                {
                          "data": "Properties",
                          "attr": {
                                    id: uri.replace(/[^a-zA-Z0-9]+/g, ''),
                                    rel: "root"
                                  },
                          "state": "closed",
                          "children": [
                            {
                                      "data": "Data Properties",
                                      "attr": {
                                                id: 'httpwwww3org200207owltopDataProperty',
                                                rel: 'topDataProperty'
                                              },
                                      "state": "closed"
                                    },
                            {
                                      "data": "Object Properties",
                                      "attr": {
                                                id: 'httpwwww3org200207owltopObjectProperty',
                                                rel: 'topObjectProperty'
                                              },
                                      "state": "closed"
                                    },
                            {
                                      "data": "Annotation Properties",
                                      "attr": {
                                                id: 'httpwwww3org200207owltopAnnotationProperty',
                                                rel: 'topAnnotationProperty'
                                              },
                                      "state": "closed"
                                    }
                          ]
                        }
              ],
              "ajax": {
                        type: "POST",
                        url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                        dataType: "json",
                        "data": function(n)
                        {
                          switch(n[0].id)
                          {
                            case "httpwwww3org200207owltopDataProperty":
                              return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                     "&method=" + "post" +
                                     "&accept=" + escape("application/json") +
                                     "&params=" + 
                                       escape( "function=getSubProperties" + 
                                               "&reasoner=" + useReasoner + 
                                               "&ontology=" + escape(ontology) + 
                                               "&parameters=" + escape("mode=descriptions") + ";" 
                                                              + escape("uri=" + propertiesTreeNodeBinder[n[0].id]) + ";" 
                                                              + escape("direct=1") + ";"
                                                              + escape("type=dataproperty")));                                
                              break;

                            case "httpwwww3org200207owltopObjectProperty":
                              return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                     "&method=" + "post" +
                                     "&accept=" + escape("application/json") +
                                     "&params=" + 
                                       escape( "function=getSubProperties" + 
                                               "&reasoner=" + useReasoner + 
                                               "&ontology=" + escape(ontology) + 
                                               "&parameters=" + escape("mode=descriptions") + ";" 
                                                              + escape("uri=" + propertiesTreeNodeBinder[n[0].id]) + ";" 
                                                              + escape("direct=1") + ";"
                                                              + escape("type=objectproperty")));                                
                              break;

                            case "httpwwww3org200207owltopAnnotationProperty":
                              return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                     "&method=" + "post" +
                                     "&accept=" + escape("application/json") +
                                     "&params=" + 
                                       escape( "function=getProperties" + 
                                               "&reasoner=" + useReasoner + 
                                               "&ontology=" + escape(ontology) + 
                                               "&parameters=" + escape("mode=descriptions") + ";" 
                                                              + escape("uri=" + propertiesTreeNodeBinder[n[0].id]) + ";" 
                                                              + escape("direct=1") + ";"
                                                              + escape("type=annotationproperty")));                                
                              break;
                          }

                          switch(n[0].attributes.rel.nodeValue)
                          {
                            case "dataProperty":
                              return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                     "&method=" + "post" +
                                     "&accept=" + escape("application/json") +
                                     "&params=" + 
                                       escape( "function=getSubProperties" + 
                                               "&reasoner=" + useReasoner + 
                                               "&ontology=" + escape(ontology) + 
                                               "&parameters=" + escape("mode=descriptions") + ";" 
                                                              + escape("uri=" + propertiesTreeNodeBinder[n[0].id]) + ";" 
                                                              + escape("direct=1") + ";"
                                                              + escape("type=dataproperty")));                                                                
                              break;

                            case "objectProperty":
                              return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                     "&method=" + "post" +
                                     "&accept=" + escape("application/json") +
                                     "&params=" + 
                                       escape( "function=getSubProperties" + 
                                               "&reasoner=" + useReasoner + 
                                               "&ontology=" + escape(ontology) + 
                                               "&parameters=" + escape("mode=descriptions") + ";" 
                                                              + escape("uri=" + propertiesTreeNodeBinder[n[0].id]) + ";" 
                                                              + escape("direct=1") + ";"
                                                              + escape("type=objectproperty")));                                
                              break;

                            case "annotationProperty":
                              return("ws=" + network.replace(/\/+$/,"") + "/ontology/read/" +
                                     "&method=" + "post" +
                                     "&accept=" + escape("application/json") +
                                     "&params=" + 
                                       escape( "function=getProperties" + 
                                               "&reasoner=" + useReasoner + 
                                               "&ontology=" + escape(ontology) + 
                                               "&parameters=" + escape("mode=descriptions") + ";" 
                                                              + escape("uri=" + propertiesTreeNodeBinder[n[0].id]) + ";" 
                                                              + escape("direct=1") + ";"
                                                              + escape("type=annotationproperty")));                                
                              break;
                          }
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
                            propertiesTreeNodeBinder[resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g,
                              '')] = resultset.subjects[i].uri;

                            var nodeType = "default";

                            switch(resultset.unprefixize(resultset.subjects[i].type).replace(/[^a-zA-Z0-9]+/g,
                              ''))
                            {
                              case "httpwwww3org200207owlDatatypeProperty":
                                nodeType = "dataProperty";
                                break;

                              case "httpwwww3org200207owlObjectProperty":
                                nodeType = "objectProperty";
                                break;

                              case "httpwwww3org200207owlAnnotationProperty":
                                nodeType = "annotationProperty";
                                break;
                            }

// Check if it is a super class of another class. If not, then it is a leaf node in the tree control.
                            if(resultset.subjects[i].getPredicateValues(
                              "http://umbel.org/umbel#superPropertyOf").length <= 0)
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

                          return (nodes);
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
                                                        "&datasets=" + escape(ontology) +
                                                        "&attributes=" + "all" +
                                                        "&page=" + "0" +
                                                        "&items=" + "20" +
                                                        "&include_aggregates=false" +
                                                        "&types=" + escape("http://www.w3.org/2002/07/owl#ObjectProperty;http://www.w3.org/2002/07/owl#DatatypeProperty;http://www.w3.org/2002/07/owl#AnnotationProperty")));                          
                        },
                        success: function(data)
                        {

                          jQuery('#messagesContainer').fadeOut("fast", function()
                          {
                            jQuery('#messagesContainer').empty();
                          });

                          resultset = new Resultset(data);

                          var root = "http://www.w3.org/2002/07/owl#TopProperty";
                          var nodes = ["#" + root.replace(/[^a-zA-Z0-9]+/g, '')];

                          for(var i = 0; i < resultset.subjects.length; i++)
                          {
                            propertiesTreeNodeBinder[resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g,
                              '')] = resultset.subjects[i].uri;
                            nodes.push("#" + resultset.subjects[i].uri.replace(/[^a-zA-Z0-9]+/g, ''));
                          }

                          if(jQuery("#searchBox").data("uri").uri != "")
                          {
                            var superPropertyType = "http://www.w3.org/2002/07/owl#topDataProperty";

                            var superProperties = jQuery.ajax({
                                      type: "POST",
                                      url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                                      dataType: "json",
                                      data: ({
                                                ws: network.replace(/\/+$/,"") + "/ontology/read/",
                                                method: "post",
                                                accept: "application/json", 
                                                params: "function=getSuperProperties" +
                                                        "&reasoner=" + useReasoner +
                                                        "&ontology=" + escape(ontology) +
                                                        "&parameters=" + escape("uri=" + jQuery("#searchBox").data("uri").uri) + ";" +
                                                                         escape("mode=descriptions") + ";" +
                                                                         escape("direct=0") + ";" +
                                                                         escape("type=dataproperty")
                                              }),
                                      async: false
                                    }).responseText;

                            //check if we got an error
                            var jsonResponseText = JSON.parse(superProperties);

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

                            if(jsonResponseText.resultset.subject.length <= 0)
                            {
                              superPropertyType = "http://www.w3.org/2002/07/owl#topObjectProperty";

                              // try to find it as an object property

                              var superProperties = jQuery.ajax({
                                        type: "POST",
                                        url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                                        dataType: "json",
                                        data: ({
                                                  ws: network.replace(/\/+$/,"") + "/ontology/read/",
                                                  method: "post",
                                                  accept: "application/json", 
                                                  params: "function=getSuperProperties" +
                                                          "&reasoner=" + useReasoner +
                                                          "&ontology=" + escape(ontology) +
                                                          "&parameters=" + escape("uri=" + jQuery("#searchBox").data("uri").uri) + ";" +
                                                                           escape("mode=descriptions") + ";" +
                                                                           escape("direct=0") + ";" +
                                                                           escape("type=objectproperty")
                                                }),
                                        async: false
                                      }).responseText;

                              //check if we got an error
                              var jsonResponseText = JSON.parse(superProperties);

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
                            }

                            if(jsonResponseText.resultset.subject.length <= 0)
                            {
                              /**
                              * If there is not object property and not data property, then it should be
                              * an annotation property
                              */
                              superPropertyType = "http://www.w3.org/2002/07/owl#topAnnotationProperty";
                              
                              return([
                                "#" + superPropertyType.replace(/[^a-zA-Z0-9]+/g, ''), 
                                "#" + jQuery("#searchBox").data("uri").uri.replace(/[^a-zA-Z0-9]+/g, '')
                              ]);
                            }                                    
                            
                            resultsetSuperProperties = new Resultset(jsonResponseText);

                            var pathProperties =
                              getPropertiesTreeOrderedPath(resultsetSuperProperties, superPropertyType);
                            var path = [];

                            for(var i = 0; i < pathProperties.length; i++)
                            {
                              propertiesTreeNodeBinder[pathProperties[i].replace(/[^a-zA-Z0-9]+/g,
                                '')] = pathProperties[i];
                              path.push("#" + pathProperties[i].replace(/[^a-zA-Z0-9]+/g, ''));
                            }

                            return (path);
                          }
                        },
                        dataType: "json"
                      }
            },
    "ui": {
              "select_limit": 1,
              /*        
                      "select_limit" : -1,
                      "select_multiple_modifier" : "ctrl",
              */
              "selected_parent_close": "select_parent",
              "initially_select": [uri.replace(/[^a-zA-Z0-9]+/g, '')]
            },
    "types": {
              "types": {
                        "root": {
                                  "valid_children": ["topDataProperty", "topObjectProperty",
      "topAnnotationProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table_gear.png'
                                          }
                                },
                        "topDataProperty": {
                                  "valid_children": ["dataProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table_gear.png'
                                          }
                                },
                        "topObjectProperty": {
                                  "valid_children": ["objectProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table_gear.png'
                                          }
                                },
                        "topAnnotationProperty": {
                                  "valid_children": ["annotationProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table_gear.png'
                                          }
                                },
                        "dataProperty": {
                                  "valid_children": ["dataProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table.png'
                                          }
                                },
                        "objectProperty": {
                                  "valid_children": ["objectProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table.png'
                                          }
                                },
                        "annotationProperty": {
                                  "valid_children": ["annotationProperty"],
                                  "icon": {
                                            "image": osf_ontologyModuleFullPath + '/imgs/table.png'
                                          },
                                  "create_node": false
                                }
                      }
            },


    /* Case insensitive sort function */
    "sort": function(a, b)
    {
      return (this.get_text(a).toLowerCase() > this.get_text(b).toLowerCase() ? 1 : -1);
    }
  };
  
  if(isAdmin)
  {
    jstreeStructure["plugins"] = ["themes", "json_data", "search", "ui", "contextmenu", "crrm", "hotkeys", "dnd", "types", "sort"];
    
    jstreeStructure["contextmenu"] = {
              "items": function(node, treeObj)
              {
                return{
                          "create": {
                                    // The item label
                                    "label": "Create new property...",
                                    // The function to execute upon a click
                                    "action": function(obj)
                                    {
                                      this.create(obj);
                                    },
                                    // All below are optional
                                    "_disabled": ('annotationProperty' == node.attr('rel')
                                      || 'root' == node.attr('rel')
      ),
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/table_add.png',
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
                                    "_disabled": ('topAnnotationProperty' == node.attr('rel')
                                      || 'topDataProperty' == node.attr('rel')
                                      || 'topObjectProperty' == node.attr('rel') || 'root' == node.attr('rel')
      ),
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/table_delete.png',
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
                                    "_disabled": ('topAnnotationProperty' == node.attr('rel')
                                      || 'topDataProperty' == node.attr('rel')
                                      || 'topObjectProperty' == node.attr('rel') || 'root' == node.attr('rel')
      ),
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/table_edit.png',
                                    "separator_before": false, // Insert a separator before the item
                                    "separator_after": false   // Insert a separator after the item
                                  },
                          "renameUri": {
                                    id: "renameUri",
                                    label: "Rename URI...",
                                    "_class": "contextMenuItem",
                                    icon: osf_ontologyModuleFullPath + '/imgs/table_gear.png',
                                    "_disabled": ('topAnnotationProperty' == node.attr('rel')
                                      || 'topDataProperty' == node.attr('rel')
                                      || 'topObjectProperty' == node.attr('rel') || 'root' == node.attr('rel')
      ),
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
                                      createPropertyUriView();
                                    }
                                  }
                        }
              }
            };
    jstreeStructure["dnd"] = {
              "drop_finish": function(data)
              {
                if(data.r.attr("id").search("structureSuperProperty")
                  != -1 || data.r.attr("id").search("structureSubProperty")
                    != -1 || data.r.attr("id").search("structureEquivalentProperty")
                      != -1 || data.r.attr("id").search("structureInverseOf")
                        != -1 || data.r.attr("id").search("comparableWith")
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
              "drop_check": function(data)
              {
                if((data.o.attr("rel") == "dataProperty"
                  && currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type)
                  == "http://www.w3.org/2002/07/owl#DatatypeProperty")
                  || (data.o.attr("rel") == "objectProperty"
                    && currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type)
                    == "http://www.w3.org/2002/07/owl#ObjectProperty"))
                {
                  return (true);
                }
                else
                {
                  return (false);
                }
              }
            };
    jstreeStructure["crrm"] = {
              "move": {
                        "default_position": "inside",
                        "check_move": function(m)
                        {
                          if(m.o.attr("rel") == "annotationProperty")
                          {
                            return (false);
                          }
                          else
                          {
                            return (true)
                          }
                        }
                      }
            };    
    
  }
  else
  {
    jstreeStructure["plugins"] = ["themes", "json_data", "search", "ui", "hotkeys", "types", "sort"];
  }
  
  jQuery("#propertiesTree").bind("loaded.jstree", function(event, data)
  {
    jQuery("#httpwwww3org200207owlTopProperty").attr("title", "");

    propertiesTreeCreated = true;
  }).jstree(jstreeStructure).bind("move_node.jstree", jQuery.proxy(function(e, data)
  {
    if(data.rslt.o.attr("rel") == "annotationProperty")
    {
      return (false);
    }

    var draggedPropertyUri = propertiesTreeNodeBinder[data.rslt.o.attr("id")];
    var parentDraggedPropertyUri = propertiesTreeNodeBinder[data.rslt.op.attr("id")];
    var targetParentPropertyUri = propertiesTreeNodeBinder[data.rslt.cr.attr("id")];

    jQuery.ajax({
              type: "POST",
              url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
              dataType: "json",
              newName: data.rslt.new_name,
              "data": {
                        ws: network.replace(/\/+$/,"") + "/ontology/read/",
                        method: "post",
                        accept: "application/json", 
                        params: "function=getProperty" +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
                                "&parameters=" + escape("uri=" + draggedPropertyUri)
                      },
              "success": function(data)
              {
                jQuery('#messagesContainer').fadeOut("slow", function()
                {
                  jQuery('#messagesContainer').empty();
                });

                // Here we read the resultset, and compose the JSON object to feed to jsTree
                resultset = new Resultset(data);

                resultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superPropertyOf");

                resultset.subjects[0].removePredicateValue("http://www.w3.org/2000/01/rdf-schema#subPropertyOf",
                  parentDraggedPropertyUri);
                resultset.subjects[0].addAttributeValue("http://www.w3.org/2000/01/rdf-schema#subPropertyOf", {
                          uri: targetParentPropertyUri
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
                                      + "&reasoner=" + useReasoner
                                  },
                          "success": function(data)
                          {
                            propertyMoveSuccess();
                          },
                          "error": function(jqXHR, textStatus, error)
                          {
                            // The Ontology: Update web service does return an empty body, and not an empty JSON object.
                            // This means that we have to rely on the HTTP status to know if the query is a success or 
                            // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
                            // string which returns a parse error.
                            if(jqXHR.status == 200)
                            {
                              propertyMoveSuccess();
                            }
                            else
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
                        params: "ontology=" + escape(ontology) + "&function=deleteProperty&parameters="
                          + escape("uri=" + propertiesTreeNodeBinder[data.args[0][0].id])
                      },
              "success": function(data)
              {
                propertyDeleteSuccess();
              },
              "error": function(jqXHR, textStatus, error)
              {
                // The Ontology: Update web service does return an empty body, and not an empty JSON object.
                // This means that we have to rely on the HTTP status to know if the query is a success or 
                // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
                // string which returns a parse error.
                if(jqXHR.status == 200)
                {
                  propertyDeleteSuccess();
                }
                else
                {                
                  jQuery('#messagesContainer').fadeOut("fast", function()
                  {
                    jQuery('#messagesContainer').empty();
                  });

                  var error = JSON.parse(jqXHR.responseText);

                  var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                  jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
                }
              }
            })
  }).bind("create.jstree", function(e, data)
  {

    var name = data.args[0].text().replace(/^\s+|\s+$/g, '');
    name = name.substring(0, name.search(/[\s]/));

    var propertyType = "";

    switch(data.rslt.obj.attr("rel"))
    {
      case "dataProperty":
        propertyType = "owl:DatatypeProperty";
        break;

      case "objectProperty":
        propertyType = "owl:ObjectProperty";
        break;

      case "annotationProperty":
        propertyType = "owl:AnnotationProperty";
        break;
    }

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
                                    type: propertyType,
                                    predicate: [
                                      {
                                                "rdfs:label": data.rslt.name
                                              },
                                      {
                                                "rdfs:subPropertyOf": {
                                                          "uri": propertiesTreeNodeBinder[data.args[0].attr("id")],
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

    createPropertyView();
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
                        params: "function=getProperty" +
                                "&reasoner=" + useReasoner +
                                "&ontology=" + escape(ontology) +
                                "&parameters=" + escape("uri=" + propertiesTreeNodeBinder[data.args[0][0].id])
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
                            propertyRenamedSuccess();
                          },
                          "error": function(jqXHR, textStatus, error)
                          {
                            // The Ontology: Update web service does return an empty body, and not an empty JSON object.
                            // This means that we have to rely on the HTTP status to know if the query is a success or 
                            // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
                            // string which returns a parse error.
                            if(jqXHR.status == 200)
                            {
                              propertyRenamedSuccess();
                            }
                            else
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

    lastSelectedPropertyNode = data.rslt.obj[0].attributes.id.nodeValue;

    switch(data.rslt.obj[0].attributes.id.nodeValue)
    {
      case "httpwwww3org200207owlTopProperty":
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function()
        {
        });

        jQuery('#ontologyViewColumRecord').empty();
        jQuery('#ontologyViewColumRecord').append("Select a sub property.");
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
        {
        });

        break;

      case "httpwwww3org200207owltopDataProperty":
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function()
        {
        });

        jQuery('#ontologyViewColumRecord').empty();
        jQuery('#ontologyViewColumRecord').append("Select a sub data property.");
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
        {
        });

        break;

      case "httpwwww3org200207owltopObjectProperty":
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function()
        {
        });

        jQuery('#ontologyViewColumRecord').empty();
        jQuery('#ontologyViewColumRecord').append("Select a sub object property.");
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
        {
        });

        break;

      case "httpwwww3org200207owltopAnnotationProperty":
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function()
        {
        });

        jQuery('#ontologyViewColumRecord').empty();
        jQuery('#ontologyViewColumRecord').append("Select a sub annotation property.");
        jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
        {
        });

        break;

      default:

        var id = data.rslt.obj[0].attributes.id.nodeValue;

        var uri = propertiesTreeNodeBinder[id];

        jQuery('#ontologyViewColumRecord').fadeTo('fast', 0.5, function()
        {
        });

        jQuery.ajax({
                  type: "POST",
                  url: structModulesBaseUrl.replace(/\/+$/,"") + "/proxy",
                  dataType: "json",
                  "data": {
                    ws: network.replace(/\/+$/,"") + "/ontology/read/",
                    method: "post",
                    accept: "application/json", 
                    params: "function=getProperty" +
                            "&reasoner=" + useReasoner +
                            "&ontology=" + escape(ontology) +
                            "&parameters=" + escape("uri=" + uri)
                  },
                  "success": function(data)
                  {

                    // Here we read the resultset, and compose the JSON object to feed to jsTree
                    currentEntityResultset = new Resultset(data);

                    createPropertyView();
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
                })

        break;
    }
  })
}

function createPropertyView()
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

  // Add the preferred label
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
    // Generate the Annotations section table
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

  if(currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type)
    != "http://www.w3.org/2002/07/owl#AnnotationProperty")
  {
    addAttributeInputAutocomplete("structureTable", "structureSubProperty", "structure", "Sub Property of",
      "http://www.w3.org/2000/01/rdf-schema#subPropertyOf", true, "http://purl.org/ontology/iron#prefLabel",
      currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type), true);
    processedAttributes.push("http://www.w3.org/2000/01/rdf-schema#subPropertyOf");

    addAttributeInputAutocomplete("structureTable", "structureSuperProperty", "structure", "Super Property of",
      "http://umbel.org/umbel#superPropertyOf", true, "http://purl.org/ontology/iron#prefLabel",
      currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type), true);
    processedAttributes.push("http://umbel.org/umbel#superPropertyOf");

    addAttributeInputAutocomplete("structureTable", "structureEquivalentProperty", "structure", "Equivalent to",
      "http://www.w3.org/2002/07/owl#equivalentProperty", true, "http://purl.org/ontology/iron#prefLabel",
      currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type), true);
    processedAttributes.push("http://www.w3.org/2002/07/owl#equivalentProperty");

    addAttributeInputAutocomplete("structureTable", "structureDisjointWith", "structure", "Disjoint with",
      "http://www.w3.org/2002/07/owl#propertyDisjointWith", true, "http://purl.org/ontology/iron#prefLabel",
      currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type), true);
    processedAttributes.push("http://www.w3.org/2002/07/owl#propertyDisjointWith");

    if(currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type)
      == "http://www.w3.org/2002/07/owl#ObjectProperty")
    {
      addAttributeInputAutocomplete("structureTable", "structureInverseOf", "structure", "Inverse of",
        "http://www.w3.org/2002/07/owl#inverseOf", true, "http://purl.org/ontology/iron#prefLabel",
        currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type), true);
      processedAttributes.push("http://www.w3.org/2002/07/owl#inverseOf");
    }
  }

  addAttributeInputAutocomplete("structureTable", "structureDomain", "structure", "Domain",
    "http://www.w3.org/2000/01/rdf-schema#domain", true, "http://purl.org/ontology/iron#prefLabel",
    "http://www.w3.org/2002/07/owl#Class", false);
  processedAttributes.push("http://www.w3.org/2000/01/rdf-schema#domain");

  if(currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type)
    != "http://www.w3.org/2002/07/owl#DatatypeProperty")
  {
    addAttributeInputAutocomplete("structureTable", "structureRange", "structure", "Range",
      "http://www.w3.org/2000/01/rdf-schema#range", true, "http://purl.org/ontology/iron#prefLabel",
      "http://www.w3.org/2002/07/owl#Class", false);
    processedAttributes.push("http://www.w3.org/2000/01/rdf-schema#range");
  }
  else
  {
    var rangeAttribute = currentEntityResultset.subjects[0].getPredicateValues('http://www.w3.org/2000/01/rdf-schema#range');
    
    var rangeDatatype = 'http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral';
    
    if(rangeAttribute.length > 0)
    {
      rangeDatatype = rangeAttribute[0].uri;
    }
    
    if(rangeDatatype != "http://www.w3.org/2001/XMLSchema#dateTime" && 
       rangeDatatype != "http://www.w3.org/2001/XMLSchema#int" && 
       rangeDatatype != "http://www.w3.org/2001/XMLSchema#float")       
    {
      // default
      rangeDatatype = "http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral";
    }
    
    var input = '';    
    
    input += '<table>';
    input += '<tr><td align="right" width="5px;" style="padding-left: 0px;"><input '+(!isAdmin ? 'disabled="disabled" ' : '')+' title="Literal" name="range_datatypes" id="range_datatype_literal" class="rangeDatatypeLiteralRadioButton" type="radio" value="http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral" '+(rangeDatatype == "http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral" ? 'checked' : '')+' /></td> <td align="left" style="padding: 0px;">Literal</td>';
    input += '<tr><td align="right" width="5px;" style="padding-left: 0px;"><input '+(!isAdmin ? 'disabled="disabled" ' : '')+' titletitle="Date" name="range_datatypes" id="range_datatype_date" class="rangeDatatypeDateRadioButton" type="radio" value="http://www.w3.org/2001/XMLSchema#dateTime" '+(rangeDatatype == "http://www.w3.org/2001/XMLSchema#dateTime" ? 'checked' : '')+' /></td> <td align="left" style="padding: 0px;">Date</td>';
    input += '<tr><td align="right" width="5px;" style="padding-left: 0px;"><input '+(!isAdmin ? 'disabled="disabled" ' : '')+' titletitle="Integer" name="range_datatypes" id="range_datatype_int" class="rangeDatatypeIntegerRadioButton" type="radio" value="http://www.w3.org/2001/XMLSchema#int" '+(rangeDatatype == "http://www.w3.org/2001/XMLSchema#int" ? 'checked' : '')+' /></td> <td align="left" style="padding: 0px;">Integer</td>';
    input += '<tr><td align="right" width="5px;" style="padding-left: 0px;"><input '+(!isAdmin ? 'disabled="disabled" ' : '')+' titletitle="Float" name="range_datatypes" id="range_datatype_float" class="rangeDatatypeFloatRadioButton" type="radio" value="http://www.w3.org/2001/XMLSchema#float" '+(rangeDatatype == "http://www.w3.org/2001/XMLSchema#float" ? 'checked' : '')+' /></td> <td align="left" style="padding: 0px;">Float</td>';
    input += '</table>';
    
    
    jQuery('#structureTable').append('<tr class="structureRow">\
                                   <td class="structureRangeText" valign="top">Range:</td>\
                                   <td valign="top">'+input+'</td>\
                                 </tr>')
        
    processedAttributes.push("http://www.w3.org/2000/01/rdf-schema#range");
  }
  
  // Add minCardinality 
  if(isAdmin)
  {  
    jQuery('#structureTable').append(' <tr class="structureRowText">\
                                         <td class="structureMinCardinalityText" valign="top">Min cardinality:</td>\
                                         <td valign="top"><input id="structureMinCardinality" class="structureMinCardinalityInputText " type="text" value=""></td>\
                                       </tr>');
                                       
    processedAttributes.push("http://purl.org/ontology/sco#minCardinality");     
    
    jQuery('#structureMinCardinality').data("attr-uri", "http://purl.org/ontology/sco#minCardinality");
    
    values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#minCardinality");
    jQuery('#structureMinCardinality').val(values);
  }
  else
  {
    processedAttributes.push("http://purl.org/ontology/sco#minCardinality");     
    
    jQuery('#structureMinCardinality').data("attr-uri", "http://purl.org/ontology/sco#minCardinality");
    
    values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#minCardinality");
    
    if(values.length > 0)
    {
      jQuery('#structureTable').append(' <tr class="structureRowText">\
                                         <td class="structureMinCardinalityText" valign="top">Min cardinality:</td>\
                                         <td id="structureMinCardinality" valign="top"></td>\
                                       </tr>');
      jQuery('#structureMinCardinality').text(values[0]);    
    }
  }  
  
  // Add maxCardinality 
  if(isAdmin)
  {
    jQuery('#structureTable').append(' <tr class="structureRowText">\
                                         <td class="structureMaxCardinalityText" valign="top">Max cardinality:</td>\
                                         <td valign="top"><input id="structureMaxCardinality" class="structureMaxCardinalityInputText " type="text" value=""></td>\
                                       </tr>');

    processedAttributes.push("http://purl.org/ontology/sco#maxCardinality");                                     

    jQuery('#structureMaxCardinality').data("attr-uri", "http://purl.org/ontology/sco#maxCardinality");

    values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#maxCardinality");
    jQuery('#structureMaxCardinality').val(values);
  }
  else
  {
    processedAttributes.push("http://purl.org/ontology/sco#maxCardinality");                                     
    
    values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#maxCardinality");
    
    if(values.length > 0)
    {
      jQuery('#structureTable').append(' <tr class="structureRowText">\
                                           <td class="structureMaxCardinalityText" valign="top">Max cardinality:</td>\
                                           <td id="structureMaxCardinality" valign="top"></td>\
                                         </tr>');      
      jQuery('#structureMaxCardinality').text(values[0]);
    }
  }

  
  // Add absolute cardinality 
  if(isAdmin)
  {
    jQuery('#structureTable').append(' <tr class="structureRowText">\
                                         <td class="structureCardinalityText" valign="top">Absolute cardinality:</td>\
                                         <td valign="top"><input id="structureCardinality" class="structureCardinalityInputText " type="text" value=""></td>\
                                       </tr>');

    processedAttributes.push("http://purl.org/ontology/sco#cardinality");                                     

    jQuery('#structureCardinality').data("attr-uri", "http://purl.org/ontology/sco#cardinality");

    values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#cardinality");
    jQuery('#structureCardinality').val(values);
  }
  else
  {
    processedAttributes.push("http://purl.org/ontology/sco#cardinality");                                     

    jQuery('#structureCardinality').data("attr-uri", "http://purl.org/ontology/sco#cardinality");

    values = currentEntityResultset.subjects[0].getPredicateValues("http://purl.org/ontology/sco#cardinality");
    
    if(values.length > 0)
    {
      jQuery('#structureTable').append(' <tr class="structureRowText">\
                                           <td class="structureCardinalityText" valign="top">Absolute cardinality:</td>\
                                           <td id="structureCardinality" valign="top"></td>\
                                         </tr>');      
      
      jQuery('#structureCardinality').val(values[0]);
    }
  }

                                     
  if(currentEntityResultset.unprefixize(currentEntityResultset.subjects[0].type)
    != "http://www.w3.org/2002/07/owl#AnnotationProperty")
  {
    if(isAdmin)
    {
      // Display advanced options
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
      })

      jQuery("#advancedTable").append(
        '<tr class="advancedRow"><td class="advancedSectionHeader" colspan="2">Drupal Settings</td></tr>');

      addAttributeInputAutocomplete("advancedTable", "fieldType", "advanced", "Field Type",
        "http://purl.org/ontology/drupal#fieldType", false, "http://purl.org/ontology/iron#prefLabel",
        "http://purl.org/ontology/drupal#FieldType", false);
      processedAttributes.push("http://purl.org/ontology/drupal#fieldType");
        
      jQuery("#advancedTable").append(
        '<tr class="advancedRow"><td class="advancedSectionHeader" colspan="2">Semantic Components Settings</td></tr>');

      addAttributeInputAutocomplete("advancedTable", "displayControl", "advanced", "Display component",
        "http://purl.org/ontology/sco#displayControl", false, "http://purl.org/ontology/iron#prefLabel",
        "http://purl.org/ontology/sco#Component", false);
      processedAttributes.push("http://purl.org/ontology/sco#displayControl");
      
      addAttributeInputAutocomplete("advancedTable", "ignoredBy", "advanced", "Ignored by",
        "http://purl.org/ontology/sco#ignoredBy", false, "http://purl.org/ontology/iron#prefLabel",
        "http://purl.org/ontology/sco#Component", false);
      processedAttributes.push("http://purl.org/ontology/sco#ignoredBy");      

      addAttributeInputAutocomplete("advancedTable", "comparableWith", "advanced", "Comparable with",
        "http://purl.org/ontology/sco#comparableWith", false, "http://purl.org/ontology/iron#prefLabel",
        "http://purl.org/ontology/sco#Component", true);
      processedAttributes.push("http://purl.org/ontology/sco#comparableWith");

      addAttributeInput("advancedTable", "shortLabel", "advanced", "Short label",
        "http://purl.org/ontology/sco#shortLabel")
      processedAttributes.push("http://purl.org/ontology/sco#shortLabel");

      addAttributeInput("advancedTable", "orderingValue", "advanced", "Ordering value",
        "http://purl.org/ontology/sco#orderingValue")
      processedAttributes.push("http://purl.org/ontology/sco#orderingValue");

      addAttributeInputAutocomplete("advancedTable", "unitType", "advanced", "Unit Type",
        "http://purl.org/ontology/sco#unitType", false, "http://purl.org/ontology/iron#prefLabel",
        "http://www.w3.org/2002/07/owl#Class", false);
      processedAttributes.push("http://purl.org/ontology/sco#unitType");

      jQuery("#advancedTable").append(
        '<tr class="advancedRow"><td class="advancedSectionHeader" colspan="2">UMBEL Settings</td></tr>');

      addAttributeInputAutocomplete("advancedTable", "isAbout", "advanced", "Is about",
        "http://umbel.org/umbel#isAbout", false, "http://purl.org/ontology/iron#prefLabel",
        "http://www.w3.org/2002/07/owl#Class", false);
      processedAttributes.push("http://umbel.org/umbel#isAbout");

      addAttributeInputAutocomplete("advancedTable", "isCharacteristicOf", "advanced", "Is characteristic of",
        "http://umbel.org/umbel#isCharacteristicOf", false, "http://purl.org/ontology/iron#prefLabel",
        "http://www.w3.org/2002/07/owl#Class", false);
      processedAttributes.push("http://umbel.org/umbel#isCharacteristicOf");


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
              jQuery("#othersTable").append(
                '<tr><td id="othersTableHeader" colspan="2">Others <img id="othersHeaderImg" src="'
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

              
            if(isAdmin)
            {  
              // Set the data associated with all created elements.
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
  }

  if(isAdmin)
  {
    jQuery('#ontologyViewColumRecord').append(
      '<div class="buttonsContainer"><input title="Save changes" id="saveButton" type="submit" value="Save" onclick="saveProperty();" class="saveButton" /></div>');
  }

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
  {
  });
}

function createPropertyUriView()
{
  var uri = currentEntityResultset.subjects[0].uri;

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
    '<input title="Rename" id="renameButton" type="submit" value="Rename" onclick="renamePropertyUri(\''
    + uri + '\');" class="renameButton" />');

  jQuery('#ontologyViewColumRecord').fadeTo('fast', 1, function()
  {
  });
}

function renamePropertyUri(oldUri)
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
              propertyUriRenameSuccess();
            },
            "error": function(jqXHR, textStatus, error)
            {
              // The Ontology: Update web service does return an empty body, and not an empty JSON object.
              // This means that we have to rely on the HTTP status to know if the query is a success or 
              // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
              // string which returns a parse error.
              if(jqXHR.status == 200)
              {
                propertyUriRenameSuccess();
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
          })
}

function saveProperty()
{
  jQuery("#saveButton").hide();

  if(jQuery("#saveButtonAjax").length <= 0)
  {
    jQuery("#ontologyViewColumRecord").append('<div style="text-align: center; width: 100%"><img id="saveButtonAjax" src="'
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

  // Get Sub Properties relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureSubProperty_" + i).length > 0 && jQuery("#structureSubProperty_" + i).val().length > 0)
    {
      var subClassObj = {
              };

      subClassObj[jQuery('#structureSubProperty_' + i).data("attr-uri")] = {
                uri: jQuery('#structureSubProperty_' + i).data("value-uri")
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

      disjointWithObj[jQuery('#structureDisjointWith_' + i).data("attr-uri")] = {
                uri: jQuery('#structureDisjointWith_' + i).data("value-uri")
              };

      predicates.push(disjointWithObj);
    }
    else
    {
      break;
    }
  }

  // Get Equivalent property with relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureEquivalentProperty_" + i).length > 0 && jQuery("#structureEquivalentProperty_" + i).val().length > 0)
    {
      var equivalentClassObj = {
              };

      equivalentClassObj[jQuery('#structureEquivalentProperty_' + i).data("attr-uri")] = {
                uri: jQuery('#structureEquivalentProperty_' + i).data("value-uri")
              };

      predicates.push(equivalentClassObj);
    }
    else
    {
      break;
    }
  }

  // Get Inverse of relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureInverseOf_" + i).length > 0 && jQuery("#structureInverseOf_" + i).val().length > 0)
    {
      var inverseOfObj = {
              };

      inverseOfObj[jQuery('#structureInverseOf_' + i).data("attr-uri")] = {
                uri: jQuery('#structureInverseOf_' + i).data("value-uri")
              };

      predicates.push(inverseOfObj);
    }
    else
    {
      break;
    }
  }

  // Get domain relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureDomain_" + i).length > 0 && jQuery("#structureDomain_" + i).val().length > 0)
    {
      var domainObj = {
              };

      domainObj[jQuery('#structureDomain_' + i).data("attr-uri")] = {
                uri: jQuery('#structureDomain_' + i).data("value-uri")
              };

      predicates.push(domainObj);
    }
    else
    {
      break;
    }
  }

  // Get range relationship(s)
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureRange_" + i).length > 0 && jQuery("#structureRange_" + i).val().length > 0)
    {
      var rangeObj = {
              };

      rangeObj[jQuery('#structureRange_' + i).data("attr-uri")] = {
                uri: jQuery('#structureRange_' + i).data("value-uri")
              };

      predicates.push(rangeObj);
    }
    else
    {
      break;
    }
  }
  
  // Get datatype range relationship
  if(jQuery('#range_datatype_literal').length > 0)
  {
    var rangeObj = {};

    if(jQuery('#range_datatype_literal').is(':checked'))
    {
      rangeObj['http://www.w3.org/2000/01/rdf-schema#range'] = {uri: 'http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral'};
      predicates.push(rangeObj);    
    }
    else if(jQuery('#range_datatype_date').is(':checked'))
    {
      rangeObj['http://www.w3.org/2000/01/rdf-schema#range'] = {uri: 'http://www.w3.org/2001/XMLSchema#dateTime'};
      predicates.push(rangeObj);          
    }
    else if(jQuery('#range_datatype_int').is(':checked'))
    {
      rangeObj['http://www.w3.org/2000/01/rdf-schema#range'] = {uri: 'http://www.w3.org/2001/XMLSchema#int'};
      predicates.push(rangeObj);          
    }
    else
    {
      rangeObj['http://www.w3.org/2000/01/rdf-schema#range'] = {uri: 'http://www.w3.org/2001/XMLSchema#float'};
      predicates.push(rangeObj);          
    }
  }

  // Get displayControl relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#displayControl_" + i).length > 0 && jQuery("#displayControl_" + i).val().length > 0)
    {
      var displayControlClassObj = {
              };

      displayControlClassObj[jQuery('#displayControl_' + i).data("attr-uri")] = {
                uri: jQuery('#displayControl_' + i).data("value-uri")
              };

      predicates.push(displayControlClassObj);
    }
    else
    {
      break;
    }
  }
  
  // Get fieldType relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#fieldType_" + i).length > 0 && jQuery("#fieldType_" + i).val().length > 0)
    {
      var fieldTypeClassObj = {
              };

      fieldTypeClassObj[jQuery('#fieldType_' + i).data("attr-uri")] = {
                uri: jQuery('#fieldType_' + i).data("value-uri")
              };

      predicates.push(fieldTypeClassObj);
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

  // Get comparable with relationships
  for(i = 0; i < 512; i++)
  {
    if(jQuery("#comparableWith_" + i).length > 0 && jQuery("#comparableWith_" + i).val().length > 0)
    {
      var comparableWithObj = {
              };

      comparableWithObj[jQuery('#comparableWith_' + i).data("attr-uri")] = {
                uri: jQuery('#comparableWith_' + i).data("value-uri")
              };

      predicates.push(comparableWithObj);
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
  
  // Get minCardinality relationships
  if(jQuery('#structureMinCardinality').val() != "")
  {
    var minCardinalityObj = {};
    minCardinalityObj[jQuery('#structureMinCardinality').data("attr-uri")] = jQuery('#structureMinCardinality').val();
    predicates.push(minCardinalityObj);
  }
  
  // Get maxCardinality relationships
  if(jQuery('#structureMaxCardinality').val() != "")
  {
    var maxCardinalityObj = {};
    maxCardinalityObj[jQuery('#structureMaxCardinality').data("attr-uri")] = jQuery('#structureMaxCardinality').val();
    predicates.push(maxCardinalityObj);
  }
  
  // Get absolute cardinality relationships
  if(jQuery('#structureCardinality').val() != "")
  {
    var cardinalityObj = {};
    cardinalityObj[jQuery('#structureCardinality').data("attr-uri")] = jQuery('#structureCardinality').val();
    predicates.push(cardinalityObj);
  }

  // Get ordering value relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#orderingValue_" + i).length > 0 && jQuery("#orderingValue_" + i).val().length > 0)
    {
      var orderingValueObj = {
              };

      orderingValueObj[jQuery('#orderingValue_' + i).data("attr-uri")] = jQuery('#orderingValue_' + i).val();
      predicates.push(orderingValueObj);
    }
    else
    {
      break;
    }
  }
  
  // Get unit type relationships
  for(i = 0; i < 512; i++)
  {
    if(jQuery("#unitType_" + i).length > 0 && jQuery("#unitType_" + i).val().length > 0)
    {
      var unitTypeObj = {
              };

      unitTypeObj[jQuery('#unitType_' + i).data("attr-uri")] = {
                uri: jQuery('#unitType_' + i).data("value-uri")
              };

      predicates.push(unitTypeObj);
    }
    else
    {
      break;
    }
  }  

  // Get is about value relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#isAbout_" + i).length > 0 && jQuery("#isAbout_" + i).val().length > 0)
    {
      var isAboutClassObj = {
              };

      isAboutClassObj[jQuery('#isAbout_' + i).data("attr-uri")] = {
                uri: jQuery('#isAbout_' + i).data("value-uri")
              };

      predicates.push(isAboutClassObj);
    }
    else
    {
      break;
    }
  }
  

  // Get is about value relationships
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#isCharacteristicOf_" + i).length > 0 && jQuery("#isCharacteristicOf_" + i).val().length > 0)
    {
      var isCharacteristicOfClassObj = {
              };

      isCharacteristicOfClassObj[jQuery('#isCharacteristicOf_' + i).data("attr-uri")] = {
                uri: jQuery('#isCharacteristicOf_' + i).data("value-uri")
              };

      predicates.push(isCharacteristicOfClassObj);
    }
    else
    {
      break;
    }
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
          otherObj[jQuery('#inputOthers_' + i + '_' + ii).data("attr-uri")] = {
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

  // Get Super Properties relationship(s)
  // Note: the superPropertyOf relationship are converted into subPropertyOf relationships by
  //       updating these sub-properties records with the subPropertyOf relationship to this
  //       super property.
  for(i = 0; i < 256; i++)
  {
    if(jQuery("#structureSuperProperty_" + i).length > 0 && jQuery("#structureSuperProperty_" + i).val().length > 0)
    {
      var subPropertyResultset = jQuery.ajax({
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
                          "&parameters=" + escape("uri=" + jQuery('#structureSuperProperty_' + i).data("value-uri"))
                }),
                async: false
              }).responseText;

      var jsonResponseText = JSON.parse(subPropertyResultset);

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

      subPropertyResultset = new Resultset(jsonResponseText);

      // Remove the superClassOf relationship since we don't want to persist it into the ontologies store
      subPropertyResultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superPropertyOf");

      // Make sure to remove the attribute/value that we are about to introduce.
      // That way we make sure not to duplicate the relationship.
      subPropertyResultset.subjects[0].removePredicateValue("http://www.w3.org/2000/01/rdf-schema#subPropertyOf",
        currentEntityResultset.subjects[0].uri);

      // Add that class as a sub-class-of the target class being manipulated
      subPropertyResultset.subjects[0].addAttributeValue("http://www.w3.org/2000/01/rdf-schema#subPropertyOf", {
                uri: currentEntityResultset.subjects[0].uri
              });

      rset.subjects.push(subPropertyResultset.subjects[0]);
    }
    else
    {
      break;
    }
  }

  // Do the same as above, but for super-property-of relationships that have been deleted for this target property.
  for(i = 0; i < removedSuperEntityOf.length; i++)
  {
    var subPropertyResultset = jQuery.ajax({
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
                        "&parameters=" + escape("uri=" + removedSuperEntityOf[i])
              }),
              async: false
            }).responseText;

    var jsonResponseText = JSON.parse(subPropertyResultset);

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

    subPropertyResultset = new Resultset(jsonResponseText);

    // Remove the superClassOf relationship since we don't want to persist it into the ontologies store
    subPropertyResultset.subjects[0].removePredicateValues("http://umbel.org/umbel#superPropertyOf");

    // Remove the sub-class-of the target class being manipulated
    subPropertyResultset.subjects[0].removePredicateValue("http://www.w3.org/2000/01/rdf-schema#subPropertyOf",
      currentEntityResultset.subjects[0].uri);

    rset.subjects.push(subPropertyResultset.subjects[0]);
  }

  // Now add the classes or properties that could have been referenced that comes from external ontologies.
  // The goal here is to "import" them into the current ontology for future use
  for(i = 0; i < importedFromExternalOntology.length; i++)
  {
    var func = "";

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
                params: "function=" + escape(func) +
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
              propertySaveSuccess();
            },
            "error": function(jqXHR, textStatus, error)
            {
              // The Ontology: Update web service does return an empty body, and not an empty JSON object.
              // This means that we have to rely on the HTTP status to know if the query is a success or 
              // not. Otherwise, jQuery will return a JSON parse error since it tries to parse an empty
              // string which returns a parse error.
              if(jqXHR.status == 200)
              {
                propertySaveSuccess();
              }
              else
              {              
                jQuery('#messagesContainer').fadeOut("fast", function()
                {
                  jQuery('#messagesContainer').empty();

                  jQuery("#saveButtonAjax").hide();
                  jQuery("#saveButton").show();
                });

                var error = JSON.parse(jqXHR.responseText);

                var errorMsg = '[' + error.id + '] ' + error.name + ': ' + error.description;

                jQuery('#messagesContainer').append('<div class="errorBox">' + errorMsg + '</div>').hide().fadeIn("fast");
              }
            }
          })
}

function getPropertiesTreeOrderedPath(resultset, targetSuperProperty)
{
  var path = ["http://www.w3.org/2002/07/owl#TopProperty", targetSuperProperty];

  for(var i = 0; i < resultset.subjects.length; i++)
  {
    var superProperties =
      resultset.subjects[i].getPredicateValues("http://www.w3.org/2000/01/rdf-schema#subPropertyOf");

    for(var ii = 0; ii < superProperties.length; ii++)
    {
      if(superProperties[ii].uri == targetSuperProperty)
      {
        var properties = getPropertiesTreeOrderedPath(resultset, resultset.subjects[i].uri);

        for(var iii = 0; iii < properties.length; iii++)
        {
          path.push(properties[iii]);
        }
      }
    }
  }

  return (path);
}

function propertyMoveSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#messagesContainer').append(
    '<div class="successBox">Property successfully moved</div>').hide().fadeIn("fast");
}

function propertyDeleteSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery("#classesTree").jstree("refresh", "httpwwww3org200207owlTopProperty");

  jQuery('#messagesContainer').append(
    '<div class="successBox">Property successfully deleted</div>').hide().fadeIn("fast");
}

function propertyRenamedSuccess()
{
  jQuery('#messagesContainer').fadeOut("fast", function()
  {
    jQuery('#messagesContainer').empty();
  });

  jQuery('#messagesContainer').append(
    '<div class="successBox">Property successfully renamed</div>').hide().fadeIn("fast");
}

function propertyUriRenameSuccess()
{
  jQuery('#messagesContainer').append('<div class="successBox">URI successfully redefined</div>').hide().fadeIn(
  "fast");

  jQuery("#renameButtonAjax").hide();
  jQuery("#renameButton").show();

  jQuery("#propertiesTree").jstree("refresh", "httpwwww3org200207owlTopProperty");
  reloadTree = false;
}

function propertySaveSuccess()
{
  jQuery('#messagesContainer').append('<div class="successBox">Property successfully saved</div>').hide().fadeIn(
  "fast");

  jQuery("#saveButtonAjax").hide();
  jQuery("#saveButton").show();

  removedSuperEntityOf = [];

  if(reloadTree)
  {
  jQuery("#propertiesTree").jstree("refresh", "httpwwww3org200207owlTopProperty");
  reloadTree = false;
  }
}
