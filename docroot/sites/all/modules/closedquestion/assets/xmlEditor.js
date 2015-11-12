/**
 * @file
 * A generic XML editor component based on jstree_cq.
 */

/**
 * To init:
 *   jQuery('#tree_container_selector').xmlTreeEditor('init', '#editor_selector', 'xml_string', config_object);
 *
 *   Where:
 *   - #tree_container_selector: The css selector for the html container that
 *     should contain the tree component.
 *   - #editor_selector: The css selector for the html container that should get
 *     the editor components.
 *   - xml_string: The string of XML to edit.
 *   - config_object: The json object containing the editor configuration.
 *
 *
 * To get the new XML string:
 *   jQuery('#tree_container_selector').xmlTreeEditor('read');
 *
 *   Where:
 *   - #tree_container_selector: The css selector for the html container that
 *     contains the tree component.
 *
 *
 * To search for a specific item in the tree
 *   jQuery('#tree_container_selector').xmlTreeEditor('search', search_string, config);
 *
 *   Where:
 *   - #tree_container_selector: The css selector for the html container that
 *     contains the tree component.
 *   - search_string: The name of the node. This can also be a path, e.g.
 *     nodeA/nodeB will search nodeBs inside nodeAs.
 *   - config: (optional) An object with the following keys:
 *     - parent: The parent node to search in. Default is the whole tree.
 *     - includeParent: Search including the parent, not just the parents
 *       children.
 *
 *
 *
 * To add a listener:
 *   jQuery('#tree_container_selector').xmlTreeEditor('bind', event, function ());
 *
 *   Where:
 *   - #tree_container_selector: The css selector for the html container that
 *     contains the tree component.
 *   - event string: The type of event to listen for. Currently one of:
 *     - change: Called when anything in the tree changes.
 *   - function: the function be be called when an event of the given type
 *     happens. The arguments of the function depend on the event type.
 *
 */
(function (jQuery) {
  jQuery.fn.xmlTreeEditor = function () {

    if (typeof jQuery.fn.on === 'undefined') {
      /* fix for jQuery <1.7 */
      jQuery.fn.on = function (type, data, fn) {
        // Handle object literals
        if (typeof type === "object") {
          for (var key in type) {
            this[ name ](key, data, type[key], fn);
          }
          return this;
        }

        if (jQuery.isFunction(data) || data === false) {
          fn = data;
          data = undefined;
        }

        var handler = name === "one" ? jQuery.proxy(fn, function (event) {
          jQuery(this).unbind(event, handler);
          return fn.apply(this, arguments);
        }) : fn;

        if (type === "unload" && name !== "one") {
          this.one(type, data, fn);

        } else {
          for (var i = 0, l = this.length; i < l; i++) {
            jQuery.event.add(this[i], type, handler, data);
          }
        }

        return this;
      };
    }



    /**
     * The data of the current instance of the editor.
     */
    var data = this.data();
    /**
     * The configuration object. Private: use function getConfig()
     */
    var _config = data.xte_config;
    /**
     * The jQuery selector used to get the div that shows the editor controls.
     */
    var editorSelector = data.xte_editor;
    /**
     * Listeners to global events.
     */
    var listeners = data.listeners;
    /**
     * whether user edited something
     */
    var isEdited = false;
    /**
     * saveHandlers are specific to the "currently shown attribute editors" and
     * are thus cleared out when a new node is selected for editing.
     */
    var saveHandlers = data.saveHandlers;
    /**
     * A numeric id used to generate unique id's in this tree.
     */
    var nextId = data.nextId;
    /**
     * The (jQuery enhanced) tree.
     */
    var tree = this;
    if (saveHandlers === undefined) {
      saveHandlers = [];
    }

    switch (arguments[0]) {
      case 'init':

        // Clear the listeners.
        listeners = {
          change: [],
          onloadeditor: []
        };
        nextId = 1;
        editorSelector = arguments[1];
        var xmlString = jQuery.trim(arguments[2]);
        _config = arguments[3];
        initEditor(editorSelector);
        doInit(tree, xmlString, getConfig());

        /* make sure data is saved if user mouse is leaving the editor area */
        if (jQuery('#xmlJsonEditor_container').data('xmleditor.mouseleave') !== true) {

          jQuery('#xmlJsonEditor_container').on('mouseleave', function () {
            var bodyField = CQ_FindBodyField(Drupal.settings.closedquestion.language);
            bodyField.value = jQuery('#xmlJsonEditor_tree_container').xmlTreeEditor('read');
            jQuery('#xmlJsonEditor_container').data('xmleditor.mouseleave', true);
          });
        }
        break;
      case 'read':
        return treeToXml(tree);
        break;
      case 'select':
        if (jQuery('#selectedNodeUpdateButton').data('isEdited') === true) {
          if (!confirm('You have unsaved changes, continue?')) {
            break;
          }
          jQuery('#selectedNodeUpdateButton').data('isEdited', false);
        }
        var $element = arguments[1];
        if ($element[0].nodeName.toLowerCase() === 'ul') {
          /* event triggered by removal of other element, find currently selected item in tree */
          $element = tree.find('a.jstree_cq-clicked').closest('li');
        }
        createEditorFor($element);
        break;
      case 'addNode':
        if (!arguments[1]) {
          addNode();
        }
        else {
          var addType = arguments[1].toString();
          var parent = arguments[2];
          var attributes = arguments[3];
          addNode(addType, parent, attributes);
        }
        break;
      case 'delNode':
        removeSelectedNode();
        break;
      case 'saveNode':
        updateSelectedNode();
        break;
      case 'cloneNode':
        cloneSelectedNode();
        break;
      case 'search':
        return _search(arguments);
        break;
      case 'closest':
        return _closest(arguments);
        break;
      case 'bind':
        var bindType = arguments[1].toLowerCase();
        var listener = arguments[2];
        if (listeners[bindType] !== undefined) {
          listeners[bindType].push(listener);
        }
        else {
          alert(Drupal.t("unknown listener type: ") + bindType);
        }
        break;
    }

    data.xte_config = _config;
    data.xte_editor = editorSelector;
    data.listeners = listeners;
    data.saveHandlers = saveHandlers;
    data.nextId = nextId;
    this.data(data);
    return this;
    /**
     * Initialises a tree from data.
     */
    function doInit(treeContainer, xmlString, config) {
      var treeData;
      var attName;
      if (xmlString.length === 0) {
        var attributes = {};
        var rootName = config.valid_children[0];
        var rootConfig = config.types[rootName];
        if (rootConfig.attributes !== undefined) {
          checkMandatoryAttributes(attributes, rootConfig.attributes);
        }
        xmlString = "<" + config.valid_children[0]
        for (attName in attributes) {
          xmlString += " " + attName + '="' + attributes[attName] + '"';
        }
        xmlString += "/>";
      }
      treeData = questionStringToTreeObject(xmlString);
      for (var key in config.types) {
        if (config.types[key].icon !== undefined) {
          if (config.types[key].icon.image.substring(0, 1) !== '/') {
            config.types[key].icon.image = config.basePath + '/assets/' + config.types[key].icon.image;
          }
        }
      }
      var treeConfig = {
        "core": {
          "html_titles": true
        },
        "plugins": ["themes", "json_data", "ui", "types", "dnd", "crrm", "search"],
        "json_data": {},
        "ui": {
          "select_limit": "1"
        },
        "themes": {
          "theme": "classic"
        },
        "search": {},
        "types": config
      }
      treeConfig.json_data.data = treeData;
      jQuery(treeContainer).jstree_cq(treeConfig);
      treeContainer.bind("select_node.jstree_cq",
              function (tree) {
                return function (e, args) {
                  var element = jQuery(args.args[0]).parent();
                  if (element.length > 0) {
                    //jQuery("#log1").html("Last operation: " + e.type);
                    tree.xmlTreeEditor('select', element);
                  }
                }
              }(tree));

      /* enable F8 for saving */
      if (jQuery('#edit-submit').data('xmleditor.f8_init') !== true) {
        jQuery('#edit-submit').data('xmleditor.f8_init', true);
        jQuery('#edit-submit').val(jQuery('#edit-submit').val() + ' (F8)');
        jQuery(window).on('keydown', function (e) {
          if (e.which === 119) {
            jQuery('#edit-submit').click();
          }
        });
      }
    }

    /**
     * Reads out a tree and converts it back to XML
     * @param {object} treeContainer
     */
    function treeToXml(treeContainer) {
      var xmlString = '';
      if (treeContainer && treeContainer.jstree_cq && treeContainer.jstree_cq("core") && treeContainer.jstree_cq("core").get_container) {
        var container = treeContainer.jstree_cq("core").get_container();
        var root = container[0].children[0].children[0];
        var treeRoot = treeContainer.jstree_cq("core")._get_node(root);
        var xmlDoc;
        if (document.implementation && document.implementation.createDocument) {
          xmlDoc = document.implementation.createDocument("", "", null);
        }
        else {
          xmlDoc = new ActiveXObject("MSXML2.DOMDocument");
        }
        if (treeRoot.length > 0) {
          addNodeToXML(treeContainer, xmlDoc, xmlDoc, treeRoot);
        }

        xmlString = getSerializedXML(xmlDoc);
      }

      /* beautify xml */
      if (typeof cqBeautifyxml !== undefined) {
        xmlString = cqBeautifyxml(xmlString);
      }
      return xmlString;
    }

    /**
     * @param treeContainer
     *   The jQuery enhanced DOM object that contains the jstree_cq.
     * @param xmlDoc
     *   The xml document to add the node to.
     * @param parent
     *   xml element to use as the parent for the new node.
     * @param node
     *   jstree_cq tree node (jQuery extended li element) to use as basis for the
     *   new node.
     */
    function addNodeToXML(treeContainer, xmlDoc, parent, node) {
      var data = node.data().jstree_cq;
      var children;
      var child;
      var n;
      var childConfig = getConfig(node);
      if (childConfig.is_group === undefined) {
        var xmlNode = xmlDoc.createElement(data.type);
        for (var attName in data.attributes) {
          var attValue = data.attributes[attName];
          xmlNode.setAttribute(attName, attValue);
        }
        if (data.content.length > 0) {
          InnerHTMLToNode(data.content, xmlNode);
        }
        parent.appendChild(xmlNode);
        children = treeContainer.jstree_cq("core")._get_children(node);
        for (n = 0; n < children.length; n++) {
          child = treeContainer.jstree_cq("core")._get_node(children[n]);
          addNodeToXML(treeContainer, xmlDoc, xmlNode, child);
        }
      }
      else {
        children = treeContainer.jstree_cq("core")._get_children(node);
        for (n = 0; n < children.length; n++) {
          child = treeContainer.jstree_cq("core")._get_node(children[n]);
          addNodeToXML(treeContainer, xmlDoc, parent, child);
        }
      }
    }

    /**
     * Creates the DOM elements for the editor.
     */
    function initEditor(editorSelector) {
      var config = getConfig();
      var added = jQuery(editorSelector).empty().append('\n\n\
      <div id=""></div>\n\
      <fieldset id="editor_values"> \
            <legend>Update properties</legend>\n\
            <form> \n\
            <div class = "editor_values_contents" id="editor_values_contents"></div>\n\
            <div><button type = "button" id="selectedNodeUpdateButton">Save</button></div> \n\
            </form>\n\
            </fieldset>\n\
            <fieldset id = "editor_structure"> \n\
            <legend> Add / remove: </legend>\n\
            <div class = "editor_structure_contents" id = "editor_structure_contents"> \n\
            <div id = "selectedNodeAddContainer"> \n\
            \
            <img src = "' + config.basePath + '/assets/icons/add_icon.png" alt = "Add" /> \n\
            Click item to add\n\
            <ul id = "selectedNodeAddlist"> </ul>\n\
            \
            <div id = "selectedNodeDescription"> </div>\n\
            </div>\n\
            <div id = "selectedNodeRemoveContainer"> \n\
            <a id = "selectedNodeRemoveButton" href = "javascript:void(0)"> <img src = "' + config.basePath + '/assets/icons/delete_icon.png" alt = "Delete"> Remove </a> <span>item</span><br /> \n\n\n\
                 <a id = "selectedNodeCloneButton" href = "javascript:void(0)"> <img src = "' + config.basePath + '/assets/icons/clone_icon.gif" alt = "Clone"> Clone </a> <span>item</span> \n\n\
\
            </div>\n\
            </div>\n\
            </fieldset>\n            ');
      added.find('#selectedNodeRemoveButton').bind('click', function () {
        tree.xmlTreeEditor('delNode');
      });
      added.find('#selectedNodeCloneButton').bind('click', function () {
        tree.xmlTreeEditor('cloneNode');
      });
      added.find('#selectedNodeUpdateButton').bind('click', function () {
        jQuery('#selectedNodeUpdateButton').data('isEdited', false);
        tree.xmlTreeEditor('saveNode');
        //tree.xmlTreeEditor('select', tree.jstree_cq('get_selected')); //refresh the editor
      });
    }

    /**
     * Helper function. Finds nodes in the question XML structure
     * @param {array} pathAsArray The path, e.g. ['question', 'mapping'] to return all mappings
     * @param {object} config {"contentsTagNameMatch": string, "titleSrc": string, "valueSrc":
     *                           string} where contentsTagNameMatch is optional. If provided,
     *                           it looks in the node's contents to find a certain tag; titleSrc
     *                           and valueSrc are attribute names of the found node; these are
     *                           used for the return array. titleSrc can have the special value
     *                           @innerHTML to obtain the html inside the matched nodes
     * @returns {Array} Array of objects: {"title": string, "value": string}
     */
    function findNodes(pathAsArray, config) {
      var pathAsString = '[rel=' + pathAsArray.join('] [rel=') + ']';
      var $elements = jQuery(pathAsString);
      var returnObj = [], matchObj;
      var regExp;
      var match;
      /* helper function to get match value */
      var getMatchValue = function ($match, src) {
        /**
         * @param object $match A jQueryfied DOM/XML node
         * @param string src Attribute name or @innerHTML
         **/
        var returnValue = '';
        if (src === '@innerHTML') {
          /* get html */
          returnValue = $match.html();
        } else {
          /* get attribute value */
          if (jQuery.isArray(src)) {
            /* array of attributes provided */
            jQuery.each(src, function (i) {
              var attrValue = $match.attr(src[i]);
              returnValue += attrValue ? attrValue + ' ' : '';
            });
            /* remove spaces */
            returnValue = returnValue.trim();
          } else {
            /* single attribute provided */
            returnValue = $match.attr(src);
          }
        }
        return returnValue;
      };
      /**
       * Find matches in elements
       */
      $elements.each(function () {
        var $match;
        var $element = jQuery(this);
        var jstree_cqData = $element.data().jstree_cq;
        if (config.contentsTagNameMatch && jstree_cqData.content) {
          /* Grab name-value pairs from matched elements' contents */
          regExp = new RegExp('<' + config.contentsTagNameMatch + ' [^>]*>(([^<]*)(<\/' + config.contentsTagNameMatch + '>))?', 'ig');
          /* find tag inside contents */
          while ((match = regExp.exec(jstree_cqData.content)) !== null) {
            $match = jQuery(match[0]);
            matchObj = {
              "title": getMatchValue($match, config.titleSrc),
              "value": getMatchValue($match, config.valueSrc)
            };
            returnObj.push(matchObj);
          }
        } else if (jstree_cqData.attributes) {
          /* Grab name-value pairs directly from matched elements */
          matchObj = {
            "title": config.titleSrc === '@innerHTML' ? jstree_cqData.content : jstree_cqData.attributes[config.titleSrc],
            "value": jstree_cqData.attributes[config.valueSrc]
          };
          returnObj.push(matchObj);
        }
      });
      return returnObj;
    }

    /**
     * Create an editor for the selected tree item.
     *
     * @param element
     *   The LI tree element to create the editor for.
     */
    function createEditorFor(element) {
      var listenerData;
      var editor, childElement;
      var addButtonWrapper, addButtons;
      var data = element.data().jstree_cq;
      var itemConfig = getConfig(element);
      var editorElements = {};
      var title = itemConfig.title || data.type;
      emptyEditor();
      editor = jQuery("#editor_values_contents");
      addButtonWrapper = jQuery('#selectedNodeAddlist');
      jQuery('#selectedNodeRemoveContainer span').text(title);
      jQuery('#selectedNodeAddContainerTitle').text(title);
      jQuery("#selectedNodeUpdateButton").text("Save " + title);
      jQuery("#editor_values_legend").html("Edit " + title)

      editorElements.forElement = getEditorElements(element);
      /* add editor for child elements? */
      if (itemConfig.children_in_editor) {
        editorElements.forChildren = [];
        jQuery(itemConfig.children_in_editor).each(function () {
          childElement = element.children("ul").children("[rel='" + this + "']");
          editorElements.forChildren.push(getEditorElements(childElement));
        });
      }

      /* create editor */
      if (editorElements.forElement.attributeEditorElements.length > 0 || editorElements.forChildren !== undefined) {
        jQuery("#editor_values").show();
        //for the element
        _appendEditorElements(editor, editorElements.forElement);
        //for its children
        jQuery(editorElements.forChildren).each(function () {
          _appendEditorElements(editor, this);
        });
      }
      else {
        jQuery("#editor_values").hide();
      }

      /* hide delete/clone buttons for question */
      if (title === 'question' || itemConfig.isDeletable === false) {
        jQuery('#selectedNodeRemoveContainer').hide();
      } else {
        jQuery('#selectedNodeRemoveContainer').show();
      }

      //add structure buttons for element
      if (editorElements.forElement.structureButtons) {
        if (editorElements.forElement.structureButtons.addNodeOptions && editorElements.forElement.structureButtons.addNodeOptions.length > 0) {
          jQuery('#selectedNodeAddContainer').show();
          addButtons = editorElements.forElement.structureButtons.addNodeOptions;
          jQuery(addButtons).each(function () {
            addButtonWrapper.append(this);
          });
        }
        else {
          jQuery('#selectedNodeAddContainer').hide();
        }
      }

      /* call listeners */
      listenerData = {
        "config": getConfig(),
        "editor": jQuery(editorSelector),
        "editorElements": editorElements,
        "treeNode": element,
        "type": data.type
      };
      if (notifyListeners('onLoadEditor', listenerData) === false) {
        /* hide editor */
        jQuery('#editor_values_contents').hide();
      }
      else {
        /* show editor */
        jQuery('#editor_values_contents').show();
      }

      /* enable auto-save */
      jQuery("#editor_values :input, #editor_values textarea").on('change keyup', function () {
        jQuery('#selectedNodeUpdateButton').data('isEdited', true);
        jQuery('#selectedNodeUpdateButton').trigger('click');
        return true;
      });
      // If save is pushed, we need to update the node.
      saveHandlers.push(
              function (element) {

                return function () {
                  /* update title */
                  var title = createTitleForElement(element);
                  tree.jstree_cq("core").set_text(element, title);
                };
              }(element));
      var attachTo = jQuery('#xmlJsonEditor_editor');
      Drupal.attachBehaviors(attachTo);
    }

    /**
     * Adds editor elements to the editor
     * @param editor
     *   A DOM node containing the editor
     * @param editorElements
     *   An object containing the editor elements, @see getEditorElements
     */
    function _appendEditorElements(editor, editorElements) {
      /* add elements to the editor/addButtonWrapper */
      if (editorElements.attributeEditorElements) {
        editorElements.attributeEditorElements.each(function () {
          editor.append(this);
        });
      }

      if (editorElements.description) {
        jQuery(editorElements.description).insertBefore(jQuery('#editor_values'));
      }
    }

    /**
     * Returns editor elements for the selected tree item.
     *
     * @param element
     *   The LI tree element to create the editor for.
     *
     * @return object
     *   An object containing the editor for the attributes, a description, and
     *   some buttons to alter the structure (e.g. add child, remove item) with
     *   the fields:
     *   - attributeEditor: DOM-node
     *   - description: DOM-node
     *   - structureButtons: object
     *     - addNodeOptions: array of DOM-node
     */
    function getEditorElements(element) {
      var attributeEditor;
      var description;
      var attName;
      var data;
      var structureButtons;
      var itemConfig;
      var config = getConfig();
      data = element.data().jstree_cq;
      itemConfig = getConfig(element);
      if (itemConfig !== undefined) {
        attributeEditor = jQuery("<div></div>");
        for (attName in itemConfig.attributes) {
          /* the attribute can be nullified by getConfig */
          if (itemConfig.attributes[attName] !== null) {
            createAttributeEditor(element, itemConfig, data, attName, attributeEditor);
          }
        }

        if (itemConfig.content === 1) {
          createContentEditor(element, data, attributeEditor);
        }
        /* add buttons which allow user to add nodes */
        if (itemConfig.valid_children !== undefined) {
          structureButtons = {};
          structureButtons.addNodeOptions = getAddNodeOptions(itemConfig.valid_children, element);
        }

        if (itemConfig.description) {
          description = jQuery('<div class="xmlJsonEditor_form_description">' + itemConfig.description + '</div>');
        }
      }

      return {
        "attributeEditorElements": attributeEditor.children(),
        "description": description,
        "structureButtons": structureButtons
      };
    }

    /**
     * Creates a list of options for the add-node dropdown.
     *
     * @param valid_children
     *   The array of valid children to put in the dropdown.
     * @param contextElement
     *   The LI tree element to create the list for.
     */
    function getAddNodeOptions(valid_children, contextElement) {
      /* sort children */
      valid_children = valid_children.sort(function (child_a, child_b) {
        var childTypeConfigA = getConfig(child_a, contextElement);
        var childTypeConfigB = getConfig(child_b, contextElement);
        var title_a = childTypeConfigA.title !== undefined ? childTypeConfigA.title : child_a;
        var title_b = childTypeConfigB.title !== undefined ? childTypeConfigB.title : child_b;
        title_a = title_a.toLowerCase();
        title_b = title_b.toLowerCase();
        return title_a > title_b ? 1 : -1;
      });
      var numberOfValidChildren = valid_children.length;
      var addChildButton;
      var addNodeOptions = [];
      var childCount;
      var childType;
      var childTypeConfig;
      var childList;
      var maxCount;
      var title;
      var imageHTML;
      var config = getConfig();
      if (numberOfValidChildren > 0) {
        for (var i = 0; i < numberOfValidChildren; i++) {
          childType = valid_children[i];
          childTypeConfig = getConfig(childType, contextElement);
          childCount = 0;
          maxCount = 1;
          if (childTypeConfig.max_count && childTypeConfig.max_count >= 0) {
            maxCount = childTypeConfig.max_count;
            childList = contextElement.find('> ul > li[rel=' + childType + ']');
            childCount = childList.length;
          }

          if (childCount < maxCount) {
            if (!childTypeConfig.hidden || (childTypeConfig.hidden !== true || childTypeConfig.hidden.toString().toLowerCase() !== 'true')) {
              title = _ucfirst(childTypeConfig.title) || _ucfirst(childType);
              imageHTML = (childTypeConfig.icon && childTypeConfig.icon.image) ? '<img src="' + config.basePath + '/assets/' + childTypeConfig.icon.image + '" /> ' : '';
              addChildButton = jQuery('<li data-value="' + childType + '"><span class="title">' + imageHTML + title + '</span><span class="description">' + (childTypeConfig.description || "") + '</span></li>');
              addChildButton.data('xmlEditornode.description', childTypeConfig.description); //store node description
              addChildButton.on('click', function () {
                var $this = jQuery(this);
                $this.siblings().removeClass('selected');
                $this.addClass('selected');
                tree.xmlTreeEditor('addNode');
              })
              addNodeOptions[i] = addChildButton;
            }
          }
        }
      }
      return addNodeOptions;
    }

    /**
     * Creates an editor element for an attribute and adds it to the given
     * editor block.
     *
     * @param element
     *   The LI tree element of the item that the attibute belongs to.
     * @param itemConfig
     *   The config data of the item that the attibute belongs to.
     * @param itemData
     *   The data of the item that the attibute belongs to.
     * @param attName
     *   The name of the attribute to create the editor for.
     * @param editor
     *   The editor HTML dom element to add the content editor to.
     */
    function createAttributeEditor(element, itemConfig, itemData, attName, editor) {
      var attributeFormElement, optionElement, attributeWrapperElement, attributeGuiElement, attributeContainerElement, attributeLabelElement, attributeDescriptionElement;
      var attConfig = itemConfig.attributes[attName];
      var attValue = "";
      var config = getConfig();
      if (itemData.attributes[attName] !== undefined) {
        attValue = itemData.attributes[attName];
      }
      var attId = attName;
      var item;
      var childTags;
      var itemValue;
      var itemTitle;
      var title;
      var noValueFunction;
      var default_hint_on_empty = {
        'string': 'Enter text',
        'int': 'Enter a number'
      };
      if (typeof (attConfig) === "string") {
        /* no config provided, create minimalistic config */
        attConfig = {
          "type": attConfig,
          "hint_on_empty": default_hint_on_empty[attConfig]
        };
      }

      /* create wrapper element */
      attributeWrapperElement = jQuery('<div class="xmlJsonEditor_attribute"></div>');
      attributeContainerElement = jQuery('<div class="xmlJsonEditor_attribute_container"></div>');
      attributeWrapperElement.append(attributeContainerElement);
      /* create editor */
      if (attConfig.alias_of === undefined && !(attConfig.depricated === 1 && attValue.length === 0)) {
        /* Check if the attribute is deprecated. If it is, and if it is empty
         * then don't show it. */
        if (attConfig.depricated === 1) {
          attributeWrapperElement.addClass("depricated");
        }

        /* set title */
        if (attConfig.title !== undefined) {
          title = attConfig.title;
        }
        else {
          title = attName;
        }

        /* hide this editor? */
        if (attConfig.hidden && (attConfig.hidden !== true || attConfig.hidden.toString().toLowerCase() !== 'true')) {
          attributeWrapperElement.css("display", "none");
        }

        /* create label element */
        attributeLabelElement = jQuery('<label>' + title + ':</label>');
        /* get list with options if auto_values is set */
        if (attConfig.auto_values !== undefined) {
          childTags = findNodes(attConfig.auto_values.path, {
            "contentsTagNameMatch": attConfig.auto_values.contentsTagNameMatch,
            "valueSrc": attConfig.auto_values.valueSrc,
            "titleSrc": attConfig.auto_values.titleSrc
          });
          if (childTags.length > 0) {
            attConfig.values = jQuery.merge([{"title": "- select -", "value": ""}], childTags);
          } else {
            attConfig.values = [{"title": "(none found)", "value": ""}];
          }
        }

        /* create form element */
        if (jQuery.isArray(attConfig.values)) {
          /* select with pre-set values */
          attributeFormElement = jQuery('<select id="xmlEditor_' + attId + '" name="' + attId + '" size="1"></select>');
          for (var i = 0; i < attConfig.values.length; i++) {
            item = attConfig.values[i];
            if (typeof (item) === "object") {
              itemValue = item.value.toString();
              itemTitle = item.title;
            }
            else {
              itemValue = item.toString();
              itemTitle = itemValue;
            }

            optionElement = jQuery('<option value="' + itemValue + '">' + itemTitle + '</option>');
            //should this option be selected?
            if (itemValue.toLowerCase() === attValue.toLowerCase()) {
              optionElement.attr("selected", "selected");
            }

            attributeFormElement.append(optionElement);
          }

          if (childTags && childTags.length === 0) {
            attributeFormElement.attr('disabled', 'disabled');
          }
        }
        else {
          /* text element */
          attributeLabelElement = jQuery('<label>' + title + ':</label>');
          attributeFormElement = jQuery('<input id="xmlEditor_' + attId + '" type="text" value="' + attValue + '" />');
        }

        /* set default value */
        if (attConfig.default_value && !attValue) {
          attributeFormElement.val(attConfig.default_value);
        }


        /* append all elements to wrapper */
        attributeContainerElement.append(attributeLabelElement);
        attributeLabelElement.append(attributeDescriptionElement);
        attributeContainerElement.append(attributeFormElement);

        /* add extras for specific attribute types */
        if (attConfig.type === 'regexp') {
          var $toggleButton = jQuery(' <a href="javascript:void(0)"><span style="display:inline">Show tester</span><span style="display:none">Hide tester</span></a>');
          attributeContainerElement.append($toggleButton)

          var $reHelper = jQuery('<div class="xmleditor_regexpcoach">Test answer: <input type="text" placeholder="Enter answer" /> <span></span> with pattern</div>');
          attributeFormElement.data('xmleditor.regexpcoach', $reHelper);
          $reHelper.data('xmleditor.regexpcoach', $reHelper);

          var keyupHandler = function () {
            var $reHelper = attributeFormElement.data('xmleditor.regexpcoach');
            var $reHelperInput = $reHelper.children('input');
            var $reHelperFeedback = $reHelper.children('span');

            try {
              var re = new RegExp(attributeFormElement.val(), 'g');

              var testVar = $reHelperInput.val().toString();

              if (jQuery.isArray(re.exec(testVar)) === true) {
                $reHelperFeedback.html('matches');
              } else {
                $reHelperFeedback.html('<b>does not match</b>');
              }
              attributeFormElement.css('color', '');
            }
            catch (e) {
              $reHelperFeedback.html('<b style="color:red">(error in pattern)</b>');
              attributeFormElement.css('color', 'red');
            }

          };
          attributeFormElement.on('keyup', keyupHandler);
          $reHelper.children('input').on('keyup', keyupHandler);
          attributeContainerElement.append($reHelper);

          $toggleButton.on('click', function () {
            jQuery(this).children('span').toggle();
            $reHelper.toggle(500)
          })

          keyupHandler();
        }

        /* attributes can be inside group */
        if (attConfig.attributeOptionGroup) {
          attributeGuiElement = jQuery('<div class="xmlJsonEditor_attribute_gui"></div>');
          var $option = jQuery('<input type="radio" name="' + attConfig.attributeOptionGroup + '" value="' + attName + '"/>');
          if (attValue !== '' && attValue !== undefined) {
            $option.attr('checked', 'checked');
          }
          attributeWrapperElement.attr('data-attribute-group', attConfig.attributeOptionGroup);
          attributeWrapperElement.attr('data-attribute-group-value', attName);

          $option.on('click', function (e) {
            var $this = jQuery(this);

            var $attributeFormElementsInGroup = jQuery('[data-attribute-group=' + attConfig.attributeOptionGroup + ']').not('[data-attribute-group-value=' + $this.val() + ']');

            $attributeFormElementsInGroup.find(':input, textarea').val('').trigger('change');
          });
          attributeGuiElement.append($option);
          attributeContainerElement.addClass('xmlJsonEditor_attribute_gui');
          attributeFormElement.on('focus', function () {
            $option.trigger('click');
          });
          attributeWrapperElement.prepend(attributeGuiElement);
        }


        /* append wrapper element to editor */
        if (attributeContainerElement.html() !== '') {
          /* append feedback element to wrapper */
          attributeContainerElement.append('<div class="xmlEditorAttributeFeedback"></div>');
          editor.append(attributeWrapperElement);
        }


        /* set feedback? */
        if (attConfig.feedback) {
          if (attributeFormElement[0].nodeName.toString().toLowerCase() === 'select') {
            /* give feedback onchange */
            attributeFormElement.change(function () {
              //but only if the value differs from hint_on_empty
              if (!attConfig.hint_on_empty || (jQuery(this).val() === attConfig.hint_on_empty && jQuery(this).data('xmlEditor.attribute_value_set_by_user') === true)) {
                handleAttributeFeedback(jQuery(this), attConfig.feedback);
              }
            });
            //trigger event to show feedback
            attributeFormElement.change();
          }
          else {
            /* give feedback onkeyup */
            attributeFormElement.keyup(function () {
              //but only if the value differs from hint_on_empty
              if (!attConfig.hint_on_empty || (jQuery(this).val() === attConfig.hint_on_empty && jQuery(this).data('xmlEditor.attribute_value_set_by_user') === true)) {
                handleAttributeFeedback(jQuery(this), attConfig.feedback);
              }
            });
            //trigger event to show feedback
            attributeFormElement.keyup();
          }
        }

        /* add description */
        if (attConfig.description !== undefined) {
          attributeDescriptionElement = jQuery('<img class="xmlJsonEditor_help_icon" src="' + config.basePath + '/assets/icons/question_icon.png" title="' + attConfig.description + '" />');
          updateFormElementFeedback(attributeFormElement, 'description', {
            "type": "description",
            "text": attConfig.description
          });
        }

        /* add styling/css */
        if (attConfig.css) {
          if (jQuery.isArray(attConfig.css)) {
            /* array with class names */
            jQuery(attConfig.css).each(function () {
              attributeFormElement.addClass(this.toString());
            })
          }
          else {
            /* css name/value object */
            attributeFormElement.css(attConfig.css);
          }
        }

        /* automatically add a hint when the element has no value
         * @todo: make this work for non-textual form elements, like select,
         * checkboxes, etc.
         */
        if (attConfig.hint_on_empty) {
          /* add hint when no value is set right now */
          if (!attValue) {
            attributeFormElement.val(attConfig.hint_on_empty);
            attributeFormElement.data('xmlEditor.attribute_value_set_by_user', false);
            attributeFormElement.addClass('xmlEditor_empty');
          }

          /* remove hint when element gets focus */
          attributeFormElement.focus(function () {
            if (jQuery(this).val() === attConfig.hint_on_empty && attributeFormElement.data('xmlEditor.attribute_value_set_by_user') === false) {
              jQuery(this).val('');
              attributeFormElement.removeClass('xmlEditor_empty');
            }
          });
          /* remember when user does something */
          attributeFormElement.keyup(function () {
            attributeFormElement.data('xmlEditor.attribute_value_set_by_user', true);
          });
          /* add hint when element gets no value */
          noValueFunction = function () {
            if (jQuery(this).val() === "") {
              jQuery(this).val(attConfig.hint_on_empty);
              attributeFormElement.data('xmlEditor.attribute_value_set_by_user', false);
              attributeFormElement.addClass('xmlEditor_empty');
            }
          };
          attributeFormElement.blur(noValueFunction);
          attributeFormElement.change(noValueFunction);
        }

        /* element has an editor, attach a change listener to it. */
        var attEditor = jQuery(".xmlJsonEditor_attribute #xmlEditor_" + attId + "", editor);
        saveHandlers.push(
                function (element, name, attEditor, attConfig) {
                  return function () {

                    var data = element.data();
                    var newValue = attEditor.val();
                    if (newValue === attConfig.hint_on_empty && attEditor.data('xmlEditor.attribute_value_set_by_user') !== true) {
                      //remove the hint_on_empty value before node is saved
                      newValue = "";
                    }
                    if (newValue.length > 0 && newValue !== " ") {
                      data.jstree_cq.attributes[name] = newValue;
                    }
                    else {
                      delete element.data().jstree_cq.attributes[name];
                    }
                    notifyListeners('change', {
                      "element": element,
                      "what": "attirbute",
                      "which": name
                    });
                  };
                }(element, attName, attEditor, attConfig)
                );
      }
    }

    /**
     * Creates an editor element for "content" and adds it to the given editor
     * block.
     *
     * @param element
     *   The LI tree element of the item that the attibute belongs to.
     * @param data
     *   The data of the item to create the editor for.
     * @param editor
     *   The editor HTML dom element to add the content editor to.
     */
    function createContentEditor(element, data, editor) {
      var filenameContainerSelector;
      var wrapper = jQuery("<div class='xmlJsonEditor_attribute form-textarea-wrapper resizable'><label>Content:</label></div>");
      var buttonWrapper = jQuery('<div class="xmlJsonEditor_buttons" />');
      wrapper.append(buttonWrapper);

      var textarea = jQuery("<textarea rows='20' name='cq_editor_content' id='cq_editor_content' class='form-textarea img_assist resizable'></textarea>");
      var buttons = {
        "i": {
          "title": "<i>I</i>",
          "description": "Italic",
          "tag": "i",
          "selfClosing": false
        }, "b": {
          "title": "<b>B</b>",
          "description": "Bold",
          "tag": "b",
          "selfClosing": false
        },
        "p": {
          "title": "P",
          "description": "Paragraph",
          "tag": "p",
          "selfClosing": false
        },
        "sub": {
          "title": "<sub>sub</sub>",
          "description": "Subscript",
          "tag": "sub",
          "selfClosing": false
        },
        "sup": {
          "title": "<sup>sup</sup>",
          "description": "Superscript",
          "tag": "sup",
          "selfClosing": false
        },
        "link": {
          "title": "Link",
          "description": "Link",
          "tag": "a",
          "selfClosing": false,
          "attributes": [
            {
              "name": "href",
              "description": "To what webpage should this link refer to? E.g. http://www.google.nl"
            }]
        }
      };

      jQuery.each(buttons, function () {
        var buttonDef = this;
        var $button = createContentEditorButton(textarea, buttonDef.title, buttonDef.description, buttonDef.tag, buttonDef.attributes, buttonDef.selfClosing);
        buttonWrapper.append($button);
        $button.on('click', function () {
          $button.data('xmleditor.handler')(jQuery(this));
        });
      });

      buttonWrapper.append(' <span>Add</span>');
      /* add entity select */
      var $entity_select = jQuery('<select><option value="-1">Symbol...</option></select>');
      $entity_select.append('<option value="&amp;gt;">&gt;</option>');
      $entity_select.append('<option value="&amp;lt;">&lt;</option>');

      $entity_select.children().on('click', function () {
        var val = jQuery(this).val();
        if (val !== "-1") {
          _insertAtCaret(textarea, val);
          textarea.trigger('keyup');
        }

        $entity_select.val('-1');
      });
      buttonWrapper.append($entity_select);

      /* add question element select */
      var $qelement_select = jQuery('<select><option value="-1">Question element...</option></select>');
      buttonWrapper.append($qelement_select);

      buttons = {
        "inlinechoice": {
          "title": "Inline answer container",
          "description": "Inline answer container",
          "tag": "inlinechoice",
          "selfClosing": true,
          "attributes": [
            {
              "name": "id",
              "description": "An inline choice should have an unique identifier (id).\n\nWhat id should this inlince choice have?"
            },
            {
              "name": "freeform",
              "description": "Is this inline choice a select list with options or a text field?\n\nEnter 0 for select list and 1 for text field"
            }]
        }
      };

      jQuery.each(buttons, function () {
        var buttonDef = this;
        var $button = createContentEditorButton(textarea, buttonDef.title, buttonDef.description, buttonDef.tag, buttonDef.attributes, buttonDef.selfClosing, 'option');
        $qelement_select.append($button);

        $qelement_select.on('change', function () {
          var $this = jQuery(this);
          var $button = $this.children(':selected');
          $button.data('xmleditor.handler')($button);
          $this.val('-1');
        });
      });

      /* add image element select */
      var $image_select = jQuery('<select><option value="-1">Image...</option></select>');
      buttonWrapper.append($image_select);
      $image_select.append('<option value="&lt;img src=&quot;add url here&quot; title=&quot;add title here&quot; /&gt;">External image</option>');

      //add options
      if (jQuery('.file').length > 0) {
        /* default Drupal file attachments */
        filenameContainerSelector = '.file';
      } else if (jQuery('.field-type-file label.media-filename').length > 0) {
        /* Media module */
        filenameContainerSelector = '.field-type-file label.media-filename';
      }

      jQuery(filenameContainerSelector).each(function () {
        var filename = jQuery(this).text().trim();
        $image_select.append('<option value="&lt;img src=&quot;[attachurl:' + filename + ']&quot; title=&quot;' + filename + '&quot; /&gt;">' + filename + '</option>');
      });

      //add event handler to options
      $image_select.on('change', function () {
        var val = jQuery(this).val();
        if (val !== "-1") {
          _insertAtCaret(textarea, val);
        }

        $image_select.val('-1');
        textarea.trigger('keyup');
      });

      editor.append(wrapper);
      wrapper.append(textarea);
      wrapper.append('<div class="xmlEditorAttributeFeedback"></div>');


      textarea.keyup(function () {
        var xmlFromString = loadXMLFromString(jQuery(this).val(), true);
        if (xmlFromString.success === false) {
          updateFormElementFeedback(jQuery(this), 'xmlParse', {
            "type": "error",
            "text": 'Warning: This text is not well-formed, which might lead to errors. <a href="javascript:void(0)" onclick="jQuery(this).next().toggle()">Technial details</a> <span style="display:none">' + xmlFromString.errorMessage + '</span> / <a href="javascript:void(0)" onclick="jQuery(this).next().toggle()">How to fix</a> <span style="display:none">1) Make sure all tags are closed, e.g. "&lt;p&gt;...&lt;/p&gt;" 2) Fix unsupported HTML entities, e.g. "&amp;amp;rightarr;"</span>'
          })
        }
        else {
          updateFormElementFeedback(jQuery(this), 'xmlParse', undefined);
        }
      });

      var area = jQuery("#cq_editor_content", editor)[0];
      area.value = cqBeautifyxml(data.content);
      saveHandlers.push(
              function (element) {

                return function (e) {
                  var content = area.value;
                  element.data().jstree_cq.content = content;
                  notifyListeners('change', {
                    "element": element,
                    "what": "content"
                  });
                }
              }(element)
              );
    }

    function createContentEditorButton($textarea, title, description, tag, attributes, selfClosing, buttonType) {
      selfClosing = selfClosing === undefined ? false : selfClosing;

      buttonType = buttonType !== undefined ? buttonType : 'span';
      var $button = jQuery('<' + buttonType + ' class="button" title="' + description + '">' + title + '</' + buttonType + '>');
      var handler;

      $button.data('textarea', $textarea);

      if (selfClosing === true) {
        handler = function ($button) {
          var $textarea = $button.data('textarea');
          var attr = "";
          if (attributes !== undefined) {
            jQuery.each(attributes, function () {
              var description = this.description === undefined ? 'Enter the value for ' + this.name : this.description;
              attr += " " + this.name + '="';
              attr += prompt(description);
              attr += '"';
            });
          }
          _insertAtCaret($textarea, '<' + tag + attr + ' />');
          $textarea.trigger('keyup');
        };
      } else {
        handler = function ($button) {
          var $textarea = $button.data('textarea');
          var attr = "";
          if (attributes !== undefined) {
            jQuery.each(attributes, function () {
              var description = this.description === undefined ? 'Enter the value for ' + this.name : this.description;
              attr += " " + this.name + '="';
              attr += prompt(description);
              attr += '"';
            });
          }
          _insertAtCaret($textarea, '<' + tag + attr + '>', '</' + tag + '>');
          $textarea.trigger('keyup');
        };
      }

      $button.data('xmleditor.handler', handler);

      return $button;
    }

    /**
     * Creates and set a title for the element.
     *
     * @param element
     *   The LI tree element of the item that needs a title.
     */
    function createTitleForElement(element) {
      var data = element.data().jstree_cq;
      var children = tree.jstree_cq("core")._get_children(element);
      var childData = [];
      for (var i = 0; i < children.length; i++) {
        childData.push(jQuery(children[i]).data().jstree_cq);
      }
      return createTitleForData(data, childData, element);
    }

    /**
     * Updates the form element feedback
     * @param formElement
     *   DOM-node
     * @param feedbackId
     *   An unique id for this feedback (so it can be removed later on)
     * @param feedbackObject
     *   An object, {"type": string, "text": string} in which "type" is the
     *   feedback type; the function will add a class
     *   xmlEditorAttributeFeedback_<type> to both the form element and the <li>
     *   containing the "text"
     */
    function updateFormElementFeedback(formElement, feedbackId, feedbackObject) {
      formElement = jQuery(formElement);
      feedbackObject = feedbackObject || {};
      var feedbackMessage = feedbackObject.text;
      var feedbackType = feedbackObject.type;
      var ul = jQuery('<ul></ul>');
      /* get feedback object from form element */
      var feedbackWrapper = formElement.closest('.xmlJsonEditor_attribute').find('.xmlEditorAttributeFeedback');
      var formElementFeedback = formElement.data('xmlJsonEditor.feedback') || {};
      if (!feedbackMessage) {
        /* remove class from form element */
        if (formElementFeedback[feedbackId] && formElementFeedback[feedbackId].type) {

          formElement.removeClass('xmlEditorAttributeFeedback_' + formElementFeedback[feedbackId].type);
        }

        /* delete feedback object */
        delete formElementFeedback[feedbackId];
      }
      else {
        /* add feedback object */
        if (!formElementFeedback[feedbackId]) {
          formElementFeedback[feedbackId] = {};
        }

        formElementFeedback[feedbackId].text = jQuery('<li class="xmlJsonEditor_feedback_' + feedbackType + '">' + feedbackMessage + '</li>');
        formElementFeedback[feedbackId].type = feedbackType;
      }

      /* put feedback in wrapper */
      for (feedbackId in formElementFeedback) {
        ul.append(formElementFeedback[feedbackId].text);
        formElement.addClass('xmlEditorAttributeFeedback_' + formElementFeedback[feedbackId].type)
      }
      feedbackWrapper.empty();
      feedbackWrapper.append(ul);
      /* store feedback object in form element */
      formElement.data('xmlJsonEditor.feedback', formElementFeedback);
    }

    /**
     * Creates and set a title for the dataset of an element.
     *
     * @param data
     *   The data of an LI tree element of the item that needs a title.
     * @param children
     *   An array of data of the child LI tree elements of the item that needs
     *   a title.
     * @param contextElement
     *   The LI tree element to create the title for.
     */
    function createTitleForData(data, children, contextElement) {
      var type = data.type;
      var typeConfig = getConfig(type, contextElement);
      var content = data.content;
      var title;
      var shortened;
      var oldLength;
      if (typeConfig.title !== undefined) {
        title = typeConfig.title;
      }
      else {
        title = _ucfirst(type);
      }

      title = '<em>' + title + '</em>';
      if (typeConfig.atts_in_title !== undefined) {
        for (var attId in typeConfig.atts_in_title) {
          var attName = typeConfig.atts_in_title[attId];
          var attValue = data.attributes[attName];
          if (attValue !== undefined) {
            title = title + ", " + attName + ": " + attValue;
          }
        }
      }

      if (typeConfig.content !== undefined && typeConfig.content) {
        var myRexexp = new RegExp("<[/]?[^<>]*>", "g");
        // Strip out double spacings.
        shortened = content.replace(/(\t|\n|\r)/g, " ");
        do {
          oldLength = shortened.length;
          shortened = shortened.replace("  ", " ");
        } while (shortened.length !== oldLength);
        // We have an element with content. Show the first bit of content.
        if (shortened.length > 50) {
          title = title + "; " + shortened.replace(myRexexp, " ").substring(0, 45) + "...";
        }
        else {
          title = title + "; " + shortened.replace(myRexexp, " ");
        }
      }
      else if (children !== undefined && typeConfig.children_in_editor !== undefined) {
        for (var i = 0; i < typeConfig.children_in_editor.length; i++) {
          var childType = typeConfig.children_in_editor[i];
          var childConfig = getConfig(childType, contextElement);
          if (childConfig.content !== undefined && childConfig.content) {
            for (var j = 0; j < children.length; j++) {
              if (children[j].metadata) {
                var childMeta = children[j].metadata;
              }
              else {
                childMeta = children[j];
              }
              if (childMeta.type === childType) {

                // Strip out double spacings.
                shortened = childMeta.content.replace(/(\t|\n|\r)/g, " ");
                do {
                  oldLength = shortened.length;
                  shortened = shortened.replace("  ", " ");
                } while (shortened.length !== oldLength);
                myRexexp = new RegExp("<[/]?[^<>]*>", "g");
                // We have an element with content. Show the first bit of content.
                if (shortened.length > 50) {
                  title = title + "; " + shortened.replace(myRexexp, " ").substring(0, 45) + "...";
                }
                else {
                  title = title + "; " + shortened.replace(myRexexp, " ");
                }
                break;
              }
            }
          }
        }
      }

      return title;
    }

    /**
     * Clear out the content of the editor divs.
     */
    function emptyEditor() {
      var config = getConfig();
      saveHandlers = [];
      var editor = jQuery(editorSelector);
      editor.find(".xmlJsonEditor_form_description").remove();
      editor.find("#editor_values_contents").empty();
      editor.find("#selectedNodeAddlist").children().remove();
      editor.find('#editor_values').children().filter('.xmlJsonEditor_form_description').remove();
      editor.find('#selectedNodeDescription').empty();
      // If the tree is empty, show the root node items in the add dropdown.
      if (treeIsEmpty()) {
        var addNodeOptions = getAddNodeOptions(config.valid_children);
        var addButtonWrapper = jQuery('#selectedNodeAddlist');
        jQuery(addNodeOptions).each(function () {
          addButtonWrapper.append(this);
        });
      }
    }

    /**
     * Checks if the tree is empty.
     *
     * @return true if the tree is empty, false otherwise.
     */
    function treeIsEmpty() {
      var container = tree.jstree_cq("core").get_container();
      var root = container[0].children[0].children[0];
      var treeRoot = tree.jstree_cq("core")._get_node(root);
      return (treeRoot.length === 0);
    }

    /**
     * Converts a closedQuestion XML string to a jstree_cq tree object.
     */
    function questionStringToTreeObject(xmlString) {
      var xmlDoc1 = parseXml(xmlString);
      var data;
      var count = xmlDoc1.childNodes.length;
      for (var n = 0; n < count; n++) {
        var child = xmlDoc1.childNodes[n];
        if (child.nodeType === 1) {
          data = handleNode(child);
        }
      }
      return data;
    }

    /**
     * Adds the attributes of the DOM element to the target jstree_cq object.
     */
    function parseAttributes(nodeConfig, node, target) {
      var count = node.attributes.length;
      for (var n = 0; n < count; n++) {
        var attr = node.attributes[n];
        var name = attr.nodeName.toLowerCase();
        var value = attr.nodeValue;
        var attConfig = nodeConfig.attributes[name];
        if (attConfig === undefined) {
//          console.log(Drupal.t('An unknown parameter "@item" was found in an item of type "@node".\nIf you do not know what to do, contact your technical support for further assitance.', {
//            "@item": name,
//            "@node": node.nodeName
//          }));
        }
        else {
          if (attConfig.alias_of !== undefined) {
            name = attConfig.alias_of;
            attConfig = nodeConfig.attributes[name];
          }
          if (attConfig.depricated === 1) {
//            console.log(Drupal.t('A depricated parameter "@item" was found in an item of type "@node".\nPlease check all items of this type in the tree for information on how to fix this.', {
//              "@item": name,
//              "@node": node.nodeName
//            }));
          }
          if (attConfig.value_aliases !== undefined) {
            var aliasOf = attConfig.value_aliases[value.toLowerCase()];
            if (aliasOf !== undefined) {
              value = aliasOf;
            }
          }
        }
        target.metadata.attributes[name] = value;
      }
    }

    /**
     * Creates branch for grouping together similar items.
     *
     * @param type
     *   The type of the group.
     * @param title
     *   The title of the group.
     *
     * @return
     *   A json tree-branch.
     */
    function createGroup(type, title) {
      var group = {
        "data": {
          "title": '<em>' + title + '</em>',
          "icon": "folder"
        },
        "state": "open",
        "attr": {
          "rel": type
        },
        "metadata": {
          "type": type,
          "attributes": {},
          "content": ""
        },
        "children": []
      }
      return group;
    }

    /**
     * Adds the chilren of the DOM element to the target jstree_cq object.
     */
    function parseChildren(node, target) {
      var config = getConfig();
      var groups = {};
      target.children = [];
      var count = node.childNodes.length;
      for (var n = 0; n < count; n++) {
        var child = node.childNodes[n];
        switch (child.nodeType) {
          case 1:
            var data = handleNode(child);
            var childConfig = config.types[data.metadata.type];
            if (childConfig === undefined) {
              alert(Drupal.t("Unknown child: ") + data.metadata.type);
              target.children.push(data);
            }
            else {
              if (childConfig.in_group === undefined) {
                target.children.push(data);
              }
              else {
                var groupName = childConfig.in_group;
                if (groups[groupName] === undefined) {
                  var groupConfig = config.types[groupName];
                  var groupTitle = groupConfig.title === undefined ? groupName : groupConfig.title;
                  groups[groupName] = createGroup(groupName, groupTitle);
                  target.children.push(groups[groupName]);
                }
                var group = groups[groupName];
                group.children.push(data);
              }
            }
            break;
        }
      }
    }

    /**
     * Turn the XML node into a jstree_cq tree element.
     */
    function handleNode(node) {
      var target = null;
      var tagName = node.tagName.toLowerCase();
      var nodeConfig = getConfig(tagName);
      var nodeId = "xmlEditor_" + (nextId++);
      if (nodeConfig === undefined) {
        return {
          "data": {
            "title": "UNKNOWN: " + tagName
          },
          "attr": {
            "rel": tagName,
            "id": nodeId
          },
          "metadata": {
            "type": tagName,
            "attributes": {},
            "content": ""
          }
        };
      }

      if (nodeConfig.alias_of !== undefined) {
        tagName = nodeConfig.alias_of;
        if (nodeConfig.content_to_attribute !== undefined) {
          node.setAttribute(nodeConfig.content_to_attribute, getXMLNodeInnerHTML(node));
        }

        nodeConfig = getConfig(tagName);
      }

      /* do some configs */
      target = {
        "data": {
          "title": tagName
        },
        "attr": {
          "rel": tagName,
          "id": nodeId
        },
        "metadata": {
          "type": tagName,
          "attributes": {},
          "content": ""
        }
      };
      //hide node in tree?
      if (nodeConfig.hidden !== undefined && nodeConfig.hidden === 1) {
        target.attr.style = "display: none";
      }

      //parse attributes
      parseAttributes(nodeConfig, node, target);
      //do some settings
      if (nodeConfig.max_children === 0) {
        if (nodeConfig.content) {
          var content = getXMLNodeInnerHTML(node);
          target.metadata.content = content;
        }
      }
      else {
        if (nodeConfig.state === undefined) {
          target.state = "open";
        }
        else {
          target.state = nodeConfig.state;
        }
        parseChildren(node, target);
      }

      // Now we set a pretty title.
      target.data.title = createTitleForData(target.metadata, target.children);
      return target;
    }

    /**
     * Converts XML string to XML DOM
     *
     * @credits http://goessner.net/download/prj/jsonxml/
     */
    function parseXml(xml) {
      var dom = null;
      if (window.DOMParser) {
        try {
          dom = (new DOMParser()).parseFromString(xml, "text/xml");
        }
        catch (e) {
          dom = null;
        }
      }
      else if (window.ActiveXObject) {
        try {
          dom = new ActiveXObject('Microsoft.XMLDOM');
          dom.async = false;
          if (!dom.loadXML(xml)) { // parse error ..
            window.alert(dom.parseError.reason + dom.parseError.srcText);
          }
        }
        catch (e) {
          dom = null;
        }
      }
      else {
        alert(Drupal.t("cannot parse xml string!"));
      }
      return dom;
    }

    /**
     * Returns serialized XML
     *
     * @param xmlObject
     *   A xml object
     *
     * @return string
     */
    function getSerializedXML(xmlObject) {
      var serializer;
      var serialized;
      try {
        // XMLSerializer exists in current Mozilla browsers
        serializer = new XMLSerializer();
        serialized = serializer.serializeToString(xmlObject);
      }
      catch (e) {
        // Internet Explorer has a different approach to serializing XML
        serialized = xmlObject.xml;
      }

      return serialized;
    }

    /**
     * Convert a string to the content of a node.
     */
    function InnerHTMLToNode(xmlString, targetNode) {
      var dom = null;
      var xmlFromString = loadXMLFromString(xmlString, true);
      if (xmlFromString.success === false) {
        dom = targetNode.ownerDocument;
        var textNode = dom.createCDATASection(xmlString);
        targetNode.appendChild(textNode);
      }
      else {
        dom = xmlFromString.dom;
        var count = dom.childNodes[0].childNodes.length;
        for (var n = 0; n < count; n++) {
          var child = dom.childNodes[0].childNodes[n];
          targetNode.appendChild(child.cloneNode(true));
        }
      }
    }

    /**
     * Converts XML string to XML DOM object
     * @param xmlString
     *   The string to convert to XML.
     * @param isInnerXML
     *  Boolean determining whether the xmlString is a full xml string or an
     *  inner xml string.
     *
     * @return object
     *   Object with the fields:
     *   - success: boolean Indication if the conversion was successful.
     *   - dom: XML-DOM object of the given XML.
     *   - errorMessage: string Message if the conversion failed.
     */
    function loadXMLFromString(xmlString, isInnerXML) {
      var dom = null;
      var returnObject = {};
      var errorMsg;
      if (isInnerXML) {
        xmlString = "<root>" + xmlString + "</root>";
      }

      if (window.DOMParser) {
        try {
          dom = (new DOMParser()).parseFromString(xmlString, "text/xml");
          if (dom.documentElement.nodeName === "parsererror" || dom.documentElement.firstChild.nodeName === "parsererror") {
            errorMsg = dom.documentElement.firstChild.textContent;
            dom = null;
          }
        } catch (e) {
          dom = null;
          errorMsg = "Markup error detected in the text. Check if tags are closed properly (<p>...</p>) and if no strange symbols are present."
        }
      }
      else if (window.ActiveXObject) {
        try {
          dom = new ActiveXObject('Microsoft.XMLDOM');
          dom.async = false;
          if (!dom.loadXML(xmlString)) {
            errorMsg = dom.parseError.reason + dom.parseError.srcText;
            dom = null;
          }
        }
        catch (e) {
          dom = null;
          errorMsg = dom.parseError.reason + dom.parseError.srcText;
        }
      }
      else {
        errorMsg = Drupal.t("cannot parse xml string!");
      }

      if (dom !== null) {
        returnObject.success = true;
        returnObject.dom = dom;
      }
      else {
        returnObject.success = false;
        returnObject.errorMessage = errorMsg;
      }

      return returnObject;
    }


    /**
     * Convert the content of the node into a string.
     */
    function getXMLNodeInnerHTML(node) {
      if (node.childNodes.length === 0) {
        // The node has no children to return!
        return "";
      } else if (node.childNodes.length === 1 && node.childNodes[0].nodeType === 4) {
        // The node has 1 child of type CDATA
        return node.childNodes[0].data;
      }
      var tagName = node.tagName.toLowerCase();
      var myregexp = new RegExp("<[\/]?(" + tagName + ")[^><]*>", "i");
      var serialized = getSerializedXML(node);
      var shorter = serialized.substr(0, serialized.length - tagName.length - 3).replace(myregexp, "");
      return shorter;
    }

    /**
     * Checks if the attributes object contains all mandatory attributes and
     * adds them if needed.
     *
     * @param attributes
     *   Object containing the attributes of a node.
     * @param attributeConfig
     *   Object containing the attribute configuration of the node.
     */
    function checkMandatoryAttributes(attributes, attributeConfig) {
      for (var attName in attributeConfig) {
        var attConfig = attributeConfig[attName];
        if (attConfig.mandatory !== undefined && attributes[attName] === undefined) {
          attributes[attName] = attConfig.mandatory;
        }
      }
    }

    /**
     * Adds a node to the tree
     *
     * @param type
     *   String (optional) determining the type, as defined in the config.
     * @param parent
     *   DOM/Jquery node (optional) The parent to which the new node will be
     *   added.
     * @param attributes
     *   JSON attribute name/values
     * @see http://www.jstree_cq.com/documentation/crrm
     */
    function addNode(type, parent, attributes) {
      if (!type) {
        var $addChildButton = jQuery(editorSelector).find('#selectedNodeAddlist').children('.selected');
        type = $addChildButton.attr('data-value');
      }

      parent = parent || tree.jstree_cq('get_selected'); //currently selected node;

      attributes = attributes || {};
      var nodeId = "xmlEditor_" + (nextId++);
      var position = "last"; //it will be added as last child
      if (treeIsEmpty()) {
        position = 'before';
        parent = -1;
      }
      var newElementConfig = getConfig(type, parent); //@todo: check whether this will not lead to bugs, as the config of the new node is obtained in its parent's context
      var title = _ucfirst(newElementConfig.title) || _ucfirst(type);
      var newNodeConfig = {
        "attr": {
          "rel": type,
          "id": nodeId
        },
        "data": {
          "title": title
        }
      };
      // Check attributes
      if (newElementConfig.attributes) {
        checkMandatoryAttributes(attributes, newElementConfig.attributes);
      }

      // hide node in tree?
      if (newElementConfig.hidden !== undefined && newElementConfig.hidden === 1) {
        newNodeConfig.attr.style = "display: none";
      }

      var callback = function () {
        var $addChildButton = jQuery(editorSelector).find('#selectedNodeAddlist').children('.selected');
        var i, newElement = arguments[0], $newElement = jQuery(newElement);
        var auto_children = newElementConfig.auto_children;
        jQuery(newElement).data("jstree_cq", {
          "type": type,
          "attributes": attributes,
          "content": ""
        });
        /* give the element a proper title */
        tree.jstree_cq("rename_node", newElement, createTitleForElement(newElement));
        /* add auto children */
        if (auto_children) {
          for (i = 0; i < auto_children.length; i++) {
            addNode(auto_children[i], newElement);
          }
        }

        /* highlight the new element */
        var $addChildButtonClone = $addChildButton.clone();
        var addChildButtonOffset = $addChildButton.offset();
        var newElementOffset = $newElement.offset();
        var newElementHeight = $newElement.height();
        $newElement.hide();
        $addChildButtonClone.css({'background': '#efe', 'position': 'absolute'});
        $addChildButtonClone.addClass('selectedNodeAddlistLi');
        jQuery('body').append($addChildButtonClone);
        $addChildButtonClone.offset(addChildButtonOffset);

        try {
          /* jQuery <1.7 does not support this */
          $addChildButtonClone.animate({
            "top": newElementOffset.top,
            "left": newElementOffset.left,
            "width": 150,
            "height": newElementHeight
          }, 800, "easeInOutExpo", function () {
            $addChildButtonClone.remove();
            $newElement.show();
          });
        } catch (e) {
          $addChildButtonClone.remove();
          $newElement.show();
        }

        /* select the new element */
        //jQuery(newElement).children("a").click();
      };
      var skip_rename = true;
      tree.jstree_cq("create", parent, position, newNodeConfig, callback, skip_rename);
      notifyListeners('change', {
        "what": "create",
        "element": newNodeConfig
      });
    }

    /**
     * Reads the editor for the selected node and updates the values.
     */
    function updateSelectedNode() {
      for (var i = 0; i < saveHandlers.length; i++) {
        saveHandlers[i].call(null);
      }
      return true;
    }

    /**
     * Clones the selected node
     */
    function cloneSelectedNode() {
      var $selectedNode = tree.find('a.jstree_cq-clicked').closest('li');
      var $clone = $selectedNode.clone(true);

      /**
       * update clone and its children
       */

      $clone.attr('id', $clone.attr('id') + '_' + (new Date().getTime())); //give it unique id
      jQuery('a', $clone).removeClass('jstree_cq-clicked');

      /* create separate clone of jstree_cq data, to lose references to original */
      $clone.data().jstree_cq = jQuery.extend(true, {}, $selectedNode.data().jstree_cq);

      $clone.find('li').each(function () {
        var $child = jQuery(this);

        $child.attr('id', $child.attr('id') + '_' + (new Date().getTime())); //give it unique id

        /* create separate clone of jstree_cq data, to lose references to original */
        $child.data().jstree_cq = jQuery.extend(true, {}, $child.data().jstree_cq);
      });

      /**
       * append clone to parent of selected node
       */
      $selectedNode.parent().append($clone);
    }

    /**
     * Removes the currently selected node from the tree.
     */
    function removeSelectedNode() {
      tree.jstree_cq("remove");
      emptyEditor();
      notifyListeners('change', {
        "what": "remove"
      });
    }

    /**
     * Returns the given string, with the first character in upper case.
     *
     * @param str
     *   The string to uppercase the first character of.
     */
    function _ucfirst(str) {
      if (!str) {
        return null;
      }
      var f = str.charAt(0).toUpperCase();
      return f + str.substr(1);
    }

    /**
     * Notify all listeners of type "type" that an event occurred, with the
     * given data.
     *
     * @param type
     *   The type of event.
     * @param data
     *   The data of the event.
     */
    function notifyListeners(type, data) {
      var i, value, returnValue;
      type = type.toLowerCase();
      if (listeners[type]) {
        for (i = 0; i < listeners[type].length; i++) {
          value = listeners[type][i].call(null, data);
          returnValue = returnValue === false ? false : value;
        }
      }

      return returnValue;
    }

    /**
     * Adds feedback to a attribute editor form element
     *
     * @param formElement
     *  A jQueried form DOM element
     * @param feedbackConfigArray
     *  Its feedback config object
     */
    function handleAttributeFeedback(formElement, feedbackConfigArray) {
      var feedbackCount = feedbackConfigArray.length;
      var i;
      var formElementValue = formElement.val();
      var feedbackObject = {};
      var feedbackConfig;
      var match;
      var stopFound = false;
      /* add new feedback */
      for (i = 0; i < feedbackCount; i++) {
        feedbackConfig = feedbackConfigArray[i];
        match = new RegExp(feedbackConfig.match);
        if (!stopFound && match.test(formElementValue) === true) {
          /* we have a match, and are allowed to process it.
           * Now find out what to do with it
           **/
          feedbackObject.text = feedbackConfig.text;
          if (feedbackConfig.correct !== undefined) {
            if (feedbackConfig.correct === 1) {
              /* style feedback text */
              feedbackObject.type = 'correct';
              /* enable form submitting */
              jQuery('#selectedNodeUpdateButton').removeAttr('disabled');
            }
            else {
              /* style feedback text */
              feedbackObject.type = 'error';
              if (feedbackObject.fatal === 1) {
                /* prevent form from submitting */
                jQuery('#selectedNodeUpdateButton').attr('disabled', 'disabled');
              }
            }

            /* show the feedback text */
            updateFormElementFeedback(formElement, feedbackConfig.match, feedbackObject);
          }

          if (feedbackConfig.stop === 1) {
            stopFound = true;
          }
        }
        else {
          /* remove feedback */
          updateFormElementFeedback(formElement, feedbackConfig.match, undefined);
        }
      }
    }

    /**
     * Private function to handle the search option.
     *
     * @param searchArguments
     *   Array of the arguments to the search option.
     *
     * @return
     *   The result of the search.
     */
    function _search(searchArguments) {
      var searchString = searchArguments[1].toString();
      var config = searchArguments[2] === undefined ? {} : searchArguments[2];
      var cssSelectorArray = [];
      var returnObject;
      var returnArray = [];
      var parent = config.parent || tree;
      var includeParent = config.includeParent || false; //parent can also be found

      var searchStringArray = searchString.split('/');
      for (var i = 0; i < searchStringArray.length; i++) {
        cssSelectorArray.push('li[rel=' + searchStringArray[i] + ']');
      }

      if (includeParent) {
        if (parent.attr('rel') !== searchStringArray[0]) {
          return jQuery();
        }
        else if (searchStringArray.length === 1) {
          /* search string is only one node deep, return the parent */
          return jQuery(parent);
        }
        else {
          /* remove first element from css selector array, which is the parent */
          cssSelectorArray.shift();
        }
      }

      returnObject = parent.find(cssSelectorArray.join(' > ul > '));
      returnObject.each(function () { //put return object items in 'normal' array
        returnArray.push(jQuery(this));
      });
      return returnArray;
    }

    /**
     * Private function that finds the closes parent of a node, that has a
     * certain type.
     *
     * @param searchArguments
     *   Array of the arguments to the search option.
     *
     * @return
     *   The result of the search.
     */
    function _closest(searchArguments) {
      var node = jQuery(searchArguments[1]);
      var ancestorType = searchArguments[2].toString();
      var closest = node.closest('li[rel=' + ancestorType + ']');
      return closest;
    }

    /**
     * Private function to insert txt at the cursor's position in a textarea
     *
     * @param {object} $textarea jQuerified textarea
     * @param {string} text text to insert
     * @author George Claghorn http://stackoverflow.com/questions/1064089/inserting-a-text-where-cursor-is-using-javascript-jquery
     */
    function _insertAtCaret($textarea, textBefore, textAfter) {
      var txtarea = $textarea[0];
      textAfter = textAfter === undefined ? '' : textAfter;
      var scrollPos = txtarea.scrollTop;
      var caretPos = txtarea.selectionStart;

      var length = txtarea.selectionEnd - caretPos;
      var textAreaTxt = $textarea.val();

      if (length === 0) {
        $textarea.val(textAreaTxt.substring(0, caretPos) + textBefore + textAreaTxt.substring(caretPos) + textAfter);
      } else {
        $textarea.val(textAreaTxt.substring(0, caretPos) + textBefore + textAreaTxt.substring(caretPos, caretPos + length) + textAfter + textAreaTxt.substring(caretPos + length));
      }
      txtarea.scrollTop = scrollPos;
    }

    /**
     * Returns the config object
     *
     * @param nodeReference
     *  (Optional) The node type or the tree LI-node to return the config for
     * @param contextElement
     */
    function getConfig(nodeReference, contextElement) {
      var newConfig = copyObject(_config);
      var numberOfConditionSets;
      var conditionSet;
      var nodeType;
      var i;
      if (nodeReference) {
        if (typeof nodeReference === 'string') {
          nodeType = nodeReference;
        }
        else {
          nodeType = jQuery(nodeReference).data().jstree_cq.type;
          if (!contextElement) {
            contextElement = nodeReference;
          }
        }
      }

      /* is there a xml specific configuration? */
      if (contextElement && jQuery.isArray(_config.xml_specific_config)) {
        numberOfConditionSets = _config.xml_specific_config.length;
        for (i = 0; i < numberOfConditionSets; i++) {
          conditionSet = _config.xml_specific_config[i].conditions;
          if (matchConfigConditionSet(conditionSet, contextElement)) {
            /* yes, alter config */
            newConfig = mergeObjects(newConfig, _config.xml_specific_config[i].config_changes);
          }
        }
      }

      /* return full config or only for nodeType */
      if (nodeType && newConfig.types) {
        return newConfig.types[nodeType];
      }
      else {
        return newConfig;
      }
    }

    /**
     * Matches a config condition set
     *
     * @param conditionSet
     *
     * @param contextElement
     *
     * @param _type
     *   Private
     *
     * @return boolean
     */
    function matchConfigConditionSet(conditionSet, contextElement, _type) {
      var configCondition;
      var items;
      var j;
      var objectKeys;
      var sharedParent;
      var returnFlag;
      if (!jQuery.isArray(conditionSet)) {
        objectKeys = getObjectKeys(conditionSet);
        /* check whether logical operator */
        if (objectKeys.length === 1) {
          /* and/or */
          _type = objectKeys[0];
          if (matchConfigConditionSet(conditionSet[_type], contextElement, _type) === true) {
            return true;
          }
        }
        else {
          /* find a node */
          if (conditionSet.node || conditionSet.family_node) {

            /* find the node */
            if (conditionSet.node) {
              items = tree.xmlTreeEditor('search', conditionSet.node);
            } else {
              sharedParent = tree.xmlTreeEditor('closest', contextElement, conditionSet.family_node.split('/')[0]);
              items = tree.xmlTreeEditor('search', conditionSet.family_node, {
                "parent": sharedParent,
                "includeParent": true
              });
            }

            returnFlag = false;
            items.each(function () {
              var item = jQuery(this);
              var itemData = item.data().jstree_cq;
              if (itemData && itemData.attributes && conditionSet.attribute) {
                var itemAttributeValue = itemData.attributes[conditionSet.attribute];
                if (itemAttributeValue) {
                  if (itemAttributeValue.match(new RegExp(conditionSet.attributeValue, "i"))) {
                    returnFlag = true;
                    return;
                  }
                }
              }
            });
          }

          return returnFlag;
        }
      }
      else {
        /* array with conditions */
        if (_type === "or") {
          for (j = 0; j < conditionSet.length; j++) {
            configCondition = conditionSet[j];
            if (matchConfigConditionSet(configCondition, contextElement, _type) === true) {
              return true;
            }
          }
        }
        else if (_type === "and") {
          for (j = 0; j < conditionSet.length; j++) {
            configCondition = conditionSet[j];
            if (matchConfigConditionSet(configCondition, contextElement, _type) === false) {
              return false;
            }
          }
          return true;
        }
      }

      return false;
    }

    /**
     * Makes a recursive copy of an object.
     *
     * @param object
     *   The object to copy.
     *
     * @return
     *   A copy of the passed object.
     */
    function copyObject(object) {
      var target;
      var type1 = typeof object;
      switch (type1) {
        case "object":
          if (object !== null) {
            if (jQuery.isArray(object)) {
              target = [];
              for (var i = object.length - 1; i >= 0; i--) {
                target[i] = copyObject(object[i]);
              }
            }
            else {
              target = {};
              for (var key in object) {
                target[key] = copyObject(object[key]);
              }
            }
          }
          break;
        default:
          target = object;
          break;
      }
      return target;
    }
    /**
     * Recursively merges an object into a target object.
     * Object children are merged.
     * Array children overwrite the original.
     * Non-object children overwrite the original.
     * The target is returned.
     *
     * @param target
     *   The object to merge the second object into.
     * @param object2
     *   The object to merge into the target object.
     *
     * @return
     *   The target object.
     */
    function mergeObjects(target, object2) {
      if (object2 !== undefined) {
        var type2 = typeof object2;
        switch (type2) {
          case "object":
            if (jQuery.isArray(object2)) {
              target = copyObject(object2);
            }
            else {
              if (typeof target !== "object") {
                target = copyObject(object2);
              }
              else {
                for (var key2 in object2) {
                  if (object2[key2] === null) {
                    delete(target[key2]);
                  }
                  else {
                    target[key2] = mergeObjects(target[key2], object2[key2]);
                  }
                }
              }
            }
            break;
          case "array":
            target = copyObject(object2);
            break;
          default:
            target = object2;
            break;
        }
      }
      return target;
    }

    /**
     * Returns al the keys of an object, sorted in an array. Non-recursive.
     * @param obj
     *  The object
     * @returns array
     */
    function getObjectKeys(obj) {
      var key;
      var returnArray = [];
      for (key in obj) {
        returnArray.push(key);
      }

      return returnArray.sort();
    }
  }
  ;
})(jQuery);
