'use strict';

/* Directives */

angular.module('restwsSearchFacet', [])
  .directive('restwsSearchFacet', function($location) {
    return {
      restrict: 'A',
      template: '<div class="restws-facet"><h2 ng-show="facetTitle.length">{{facetTitle}}</h2><ul><li ng-repeat="item in searchFacet.items | limitTo:limit"><a href="" ng-click="activateFacet(this)"><span class="facet-remove" ng-show="item.active">&nbsp;-&nbsp;</span>{{item.text}} <span class="facet-count">({{item.count}})</span></a></li></ul><div class="expand-facet" ng-hide="facetExpanded || hideExpandCollapse" ng-click="expandFacet()">{{showMoreText}}</div><div class="collapse-facet" ng-show="facetExpanded && !hideExpandCollapse" ng-click="collapseFacet()">{{showLessText}}</div></div>',
      replace: true,
      scope: {
        facetTitle: '@',
        searchFacet: '=',
        softLimit: '@',
        hardLimit: '@',
        showMoreText: '@',
        showLessText: '@',
        facetExpanded: '@',
        hideExpandCollapse: '@'
      },
      controller: function($scope, $element, $location) {
        $scope.activateFacet = function(path) {
          $location.search(path.item.path);
        }
        
        $scope.expandFacet = function() {
          $scope.limit = $scope.hardLimit;
          $scope.facetExpanded = true;
        }
        
        $scope.collapseFacet = function() {
          $scope.limit = $scope.softLimit;
          $scope.facetExpanded = false;
        }
      },
      link: function(scope, element, attrs) {
        attrs.$observe('hideExpandCollapse', function(val) {
          if (false == angular.isDefined(val)) {
            scope.hideExpandCollapse = false;
          }
        });
        attrs.$observe('softLimit', function(val) {
          if (false == angular.isDefined(val)) {
            scope.softLimit = 25;
          }
        });
        
        attrs.$observe('hardLimit', function(val) {
          if (false == angular.isDefined(val)) {
            scope.hardLimit = 50;
          }
        });
        
        attrs.$observe('facetExpanded', function(val) {
          if (false == angular.isDefined(val)) {
            scope.facetExpanded = false;
          }
          
          scope.limit = (true == scope.facetExpanded) ? scope.hardLimit : scope.softLimit;
        });
        
        attrs.$observe('showMoreText', function(val) {
          if (false == angular.isDefined(val)) {
            scope.showMoreText = 'Show more';
          }
        });
        
        attrs.$observe('showLessText', function(val) {
          if (false == angular.isDefined(val)) {
            scope.showLessText = 'Show fewer';
          }
        });
      }
    }
  });