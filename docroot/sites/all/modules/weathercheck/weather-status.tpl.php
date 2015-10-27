<?php

/**
 * @file
 * Page to display Weather Check Data.
 */
?>
<div id="weathercheck_map"></div>

<div ng-app="weatherapp" id="YourElementId" ng-controller="MyModuleWeather">
    <h3 ng-bind="Place"></h3>
    <p ng-bind="description"/></p>
    <p ng-bind-html-unsafe="temperature"></p>
    <p ng-bind="Pressure"></p>
    <p ng-bind="Humidity"></p>
    <p ng-bind="windSpeed"></p>
    <p ng-bind="WindDir"></p>
    <p ng-bind="VisibilityArea"></p>
</div>
