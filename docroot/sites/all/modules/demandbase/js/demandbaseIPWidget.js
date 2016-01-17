/***
Name: Demandbase IP Widget
Authora: Matthew Downs (mdowns[at]demandbase[dot]com)
License: Copyright 2012. This code may not be reused without explicit permission by its owners. All rights reserved.
***/

var DemandbaseLabs = {};
DemandbaseLabs.IPWidget = {
  ipWidget : null,
  ipDetails : null,
  detectedIP : '',  //IP address detected by demandbase
  overrideIP : $('overrideIpAddress').val(),  //IP address manually entered by user
  token : 'b829ab20db2ebd2f1673dacb11a15210166c8c9a',  //this key is for use on demandbaselabs.com and its subdomains ONLY!  Violoators will be prosecuted!
  debug : false,
  dbCompanyDataSet : [],
  setIpWidgetMode : function(mode) {
    /*var date = new Date();
    date.setFullYear(date.getFullYear() + 1);
    document.cookie = 'review=' + encodeURIComponent(mode) + '; expires=' + date.toGMTString() + '; path=/';*/
    ipWidget.className = mode;
    //changes the arrow character on the toggle button...
    if (mode == 'off')
      document.getElementById('ipWidgetShowHide').value = '\u2B05'; 
    else
      document.getElementById('ipWidgetShowHide').value = '\u27A1';
  },

  setIpDetailsMode : function(mode) {
    this.ipDetails.className = mode;
  },

  showIpWidgetHelp: function() {
    //modal popup explaining the widget, triggered by 'help' button
    alert('Entering an IP Address here simulates a visitor from an alternate IP address, showing how this site would look customized by the firmographic data returned by the Demandbase IP Address API for that visitor.');
  },

  toggleIpWidgetMode : function() {
    this.setIpWidgetMode(ipWidget.className == 'off' ? 'on' : 'off');
  },

  toggleIpDetails : function() {
    this.setIpDetailsMode(ipDetails.className == 'off' ? 'on' : 'off');
  },

  simulateVisit : function() {  //called when "Simulate" button is clicked...
    this.overrideIP = $('#overrideIpAddress').val(); //get from document when simulate button is called --updating global value
    if(overrideIP != '') {
      this.overrideIP = overrideIP; //update local value
      this.setSimulateMsg();
      this.loadAsyncScript();
    } else {
      alert('Please enter an IP address');
    }
  },

  updateOverrideIp : function(element) {
    var idx=element.selectedIndex;
    var val=element.options[idx].value;
    $('#overrideIpAddress').val(val);
   this.overrideIP = $('#overrideIpAddress').val(); //get from document when simulate button is called --updating global value
  },

  initIpFields : function(company) {
    //TODO: put this into demandbase_parse function?
    if(company) {
      this.overrideIP = $('#overrideIpAddress').val();
      this.detectedIP = $('#overrideIpAddress').val();
      this.initIpWidget();
      this.setDetailFields(company);
    }
  },

  loadAsyncScript : function() {
    //TODO: ? check session variable and assign to IP field?
    //TODO: this adds an object to the page each time...can we replace existing instead?
    var s = document.createElement('script');
    s.src = "http://api.demandbase.com/api/v2/ip.json?key="+this.token+"&query="+this.overrideIP+"&callback=DemandbaseLabs.IPWidget.demandbase_override";
    document.getElementsByTagName('head')[0].appendChild(s);
  },

  flattenData : function(data) {
    //some demandbase fields are returned as json objects (hq hierarchy and account watch)
    //this fcn breaks out each individual field in those objects andnormalizes the name, making
    //it possible to iterate through the entire data set without checking for objects
    for (d in data){
      if (typeof data[d] == 'object') {
        for (nd in data[d]) {
          data[d+'_'+nd] = data[d][nd];
        }
        delete data[d];
      }
    }
    return data;
  },

  demandbase_parse : function(company) {
      //initial callback function when instantiating ipWidget via IP API
      if(company) {
          this.dbCompanyDataSet = company;
          DemandbaseLabs.IPWidget.initIpFields(company);
          
      }
  },

  demandbase_override : function(company) {
      //callback function used by loadAsyncScrip fcn
      if(company) {
          //alert(company.company_name);
          this.setDetailFields(company);
          this.dbCompanyDataSet = company; //update data object on override
      }
      if(typeof DBForms !== 'undefined') {
        //if on a forms page, also refresh/override data from form API calls
        DBForms.demandbaseParser.parser(company);
      } 
  },

  setDetailFields : function(company) {
    //this function builds a set of tabs populated with demandbase data and puts it into the widget
    var regTabToken = 'registry';
    var hqTabToken = 'hq';
    var awTabToken = 'watch_list';
    var locFieldList = ["street_address", "city", "state", "zip", "country", "country_name", "phone", "latitude", "longitude"];
    if(! company) return '';  //404 safety
    company = this.flattenData(company);   //for hq and account watch
    
    this.ipDetails.innerHTML = ''; //clear the results of the previous query
    //creating the tab switcher elements for jQuery UI (note CSS for classes lives in jQ CSS files)
    tabsElm = document.createElement('div');
    tabsElm.innerHTML = '<div id="tabs" class="ui-tabs ui-widget ui-widget-content ui-corner-all">' +
      '<ul class="ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all">' +
        '<li class="ui-state-default ui-corner-top ui-tabs-selected ui-state-active"><a href="#tabs-1">Company Summary</a></li>' +
        '<li class="ui-state-default ui-corner-top"><a href="#tabs-2">Registry</a></li>' +
        '<li class="ui-state-default ui-corner-top"><a href="#tabs-3">HQ</a></li>' +
        '<li class="ui-state-default ui-corner-top"><a href="#tabs-4">Account Watch</a></li>' +
        '<li class="ui-state-default ui-corner-top"><a href="#tabs-5">Premium</a></li>' +
      '</ul>' +
      '<div id="tabs-1" class="ui-tabs-panel ui-widget-content ui-corner-bottom"></div>' +
      '<div id="tabs-2" class="ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide"></div>' +
      '<div id="tabs-3" class="ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide"></div>' +
      '<div id="tabs-4" class="ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide"></div>' +
      '<div id="tabs-5" class="ui-tabs-panel ui-widget-content ui-corner-bottom ui-tabs-hide"></div>' +
    '</div>';
    ipDetails.appendChild(tabsElm);
    $('#tabs').tabs();  //instantiate jQ UI tab switcher

    try {
      //Set IP Source and register with sourceChecker
      //if (!data.source) data.source = 'ip';     
      //this._sourceChecker.setSource(data.source, (data.company_name ? true : false), false)
      var fs = document.createElement('ul');
      fs.id='db_data_container';
      tabID = fs.id; //initialize
      for(eachField in company){
        var elName = eachField; //this._normalize(eachField);
        var newEl = document.createElement('li');
        newEl.setAttribute('id', elName);
        newEl.setAttribute('name', elName);
        //newEl.setAttribute('type',this.elType);
        newEl.setAttribute('class', 'dbFieldValue');
        newEl.innerHTML = '<strong>'+elName +': </strong>' + company[eachField];
        if(elName.substring(0, regTabToken.length) == regTabToken) {
          tabID = 'tabs-2'; //registry fields on tab 2
        } else if (elName.substring(elName.length-hqTabToken.length, elName.length) == hqTabToken || elName.indexOf(hqTabToken) != -1 || locFieldList.indexOf(elName) != -1) {
          tabID = 'tabs-3'; //location info and ending with hq goes to tab 3
        } else if (elName.substring(0, awTabToken.length) == awTabToken) {
          tabID = 'tabs-4'; //account watch is tab 4
        } else {
          //all the important stuff on tab 1
          tabID = 'tabs-1';
        }
        tabElm = document.getElementById(tabID);
        tabElm.appendChild(newEl);         
      }

      //add "Your data here" demo messaging
      var newEl = document.createElement('li');
      newEl.setAttribute('id', 'watch_list_promo');
      newEl.setAttribute('name', 'watch_list_promo');
      newEl.setAttribute('class', 'dbFieldValue');
      newEl.innerHTML = '<br/><strong>[ <em>Your custom data</em> goes here! ]</strong>';
      var tabElm = document.getElementById('tabs-4');
      tabElm.appendChild(newEl);

      //Show [example] premium attrs on last tab for DUNS and tech insights
      tabElm = document.getElementById('tabs-5');
      var newEl = document.createElement('li');
      newEl.setAttribute('id', 'premium_duns');
      newEl.setAttribute('name', 'premium_duns');
      newEl.setAttribute('class', 'dbFieldValue');
      newEl.innerHTML = '<strong>DUNS Number</strong>';
      tabElm.appendChild(newEl);
      
      var newEl = document.createElement('li');
      newEl.setAttribute('id', 'premium_ti');
      newEl.setAttribute('name', 'premium_ti');
      newEl.setAttribute('class', 'dbFieldValue');
      newEl.innerHTML = '<strong>Technology Insights Data</strong><br/><br/><em>Talk to your CSM or Sales for more info!</em>';
      tabElm.appendChild(newEl);

      this.ipDetails.appendChild(fs);
      
    } catch(e){if(debug)alert('Error: '+e);}
    
    $('#tabs').tabs().select(1); //render with the first tab chosen
  }, //end setDetailFields

  initIpWidget : function() {
    //only build a widget if there isn't one already in the page.
    if($('#ipWidget').length == 0){
      this.ipWidget = document.createElement('div');
      this.ipWidget.id = 'ipWidget';
      this.ipWidget.onclick = function (event) { event.stopPropagation() };
      this.testIP = (this.debug) ? this.detectedIP : this.overrideIP;
      if(this.overrideIP != '' && this.overrideIP != 'null') {
        this.testIP = this.overrideIP;
      } else {
        this.testIP = this.detectedIP;
      }
      if(this.useTestIP) { testIP = TEST_IP; }
      
      //building the ip address widget ui
      if(typeof DBForms !== 'undefined') {
        toggleFldsElm = '<span><input type="checkbox" value="false" onclick="DBForms.demandbaseParser.toggleHiddenFields()">Display Hidden Form Fields</input></span>';
      } else { toggleFldsElm = ''; }
      revertElm = '<span><input type="checkbox" id="chkRevertIp" onclick="DemandbaseLabs.IPWidget.toggleOverrideIp()">Use Actual IP</input></span>';
      helpElm = '<input value="Help" type=button onclick=showIpWidgetHelp()>';
      this.ipWidget.innerHTML = '<input value="" id="ipWidgetShowHide" type=button onclick=DemandbaseLabs.IPWidget.toggleIpWidgetMode()>';
     // localStorage.setItem('overrideIpAddress', this.testIP);
      //alert('set in initIpWidget-to testIP:' + this.testIP);
      this.ipDetails = document.createElement('div');
      this.ipDetails.id = 'ipDetails';
      this.ipDetails.onclick = function (event) { event.stopPropagation() };
      this.ipDetails.innerHTML = '<h4>Demandbase IP Data</h4>';
      this.ipWidget.appendChild(this.ipDetails);
      document.body.appendChild(this.ipWidget);
      $('#ipDetails').before($('#manualIP'));
      
      //turning debug mode on, the widget loads expanded
      if(this.debug) {
        var mode = 'on';
      } else {
        var mode = 'off'; //getCookie('review');
      }
      if (mode != 'off') {
        mode = 'on';
      }
      this.setIpWidgetMode(mode);
      this.setIpDetailsMode(mode);
      manualIP = $('overrideIpAddress').val();
    }
  }  //end initIpWidget
} //end DemandbaseLabs.IPWidget



