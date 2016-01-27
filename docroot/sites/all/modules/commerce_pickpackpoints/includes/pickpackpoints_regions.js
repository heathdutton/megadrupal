/**
 *  After change shire select call this
 */
function ChangeCountyList(selCounty)
{
  // Get select objects
  var countyList = document.getElementById("counties");
  var cityList = document.getElementById("cities");
  var shopList = document.getElementById("shops");

  // Create default value if filed parameter
  if (selCounty === undefined) {
    selCounty = countyList.options[countyList.selectedIndex].value;
  }

  // Clear options
  cityList.options.length = 0;
  shopList.options.length = 0;

  // Generating option list
  var cityOpts = cities[selCounty];
  if (cityOpts)
  {
    var city = new Option(Drupal.t("Choose a city"), -1);
    cityList.options.add(city);
    for (var i = 0; i < cityOpts.length; i++)
    {
      city = new Option(cityOpts[i],i);
      cityList.options.add(city);
    }
    cityList.disabled = false;
  }
}

/**
 *  After change city select call this
 */
function ChangeCityList(selCounty, selCity)
{
  // Get select objects
  var countyList = document.getElementById("counties");
  var cityList = document.getElementById("cities");
  var shopList = document.getElementById("shops");

  // Create default value if filed parameter
  if (selCounty === undefined) {
    selCounty = countyList.options[countyList.selectedIndex].value;
  }
  if (selCity === undefined) {
    selCity = cityList.options[cityList.selectedIndex].value;
  }

  // Clear options
  shopList.options.length = 0;

  // Generating option list
  var shopOpts = shops[selCounty][selCity];
  var option;
  if (shopOpts)
  {
    var shop = new Option(Drupal.t("Choose a shop"), -1);
    shopList.options.add(shop);
    for (var i = 0; i < shopOpts.length; i++)
    {
      option = shopOpts[i];
      if (option) {
        shop = new Option(option, i);
        shopList.options.add(shop);
      }
    }
    shopList.disabled = false;
  }
}

/**
 *  After change settlemen select call this
 */
function ChangeShopList(selShop)
{
  // Get select objects
  var shopList = document.getElementById("shops");

  // Create default value if filed parameter
  if (selShop === undefined) {
    selShop = shopList.options[shopList.selectedIndex].value;
  }

  // Set shop text
  if (selShop > -1) {
    document.getElementById('pickpack-text').innerHTML = renderedShops[selShop];
  }
}

/**
 *  After loading page call this
 */
function PickPackRemoveOptions(selCounty, selCity, selShop) {
  // Get select objects
  var countyList = document.getElementById("counties");
  var cityList = document.getElementById("cities");
  var shopList = document.getElementById("shops");

  // Remove all optgroups and options from selects
  var sel = cityList;
  while (sel.firstChild) {
    sel.removeChild(sel.firstChild);
  }
  sel = shopList;
  while (sel.firstChild) {
    sel.removeChild(sel.firstChild);
  }

  // Setting all available select options
  if (selCounty > -1) {
    ChangeCountyList(selCounty);
  }
  if (selCity > -1) {
    ChangeCityList(selCounty, selCity);
  }
  if (selShop > -1) {
    ChangeShopList(selShop);
  }

  // Set selected indexes
  countyList.selectedIndex = selCounty + 1;
  cityList.selectedIndex = selCity + 1;
  shopList.selectedIndex = selShop + 1;
}
