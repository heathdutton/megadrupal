Drupal PSR Cache
================

PSR compliant cache for Drupal using the Drupal cache API.

It contains a static cache proxy in order to stack cache pools together.


Install
-------

* download the module
* run composer in the module folder: ```composer update```
* enable module


Usage
-----

Fetching a cached item:

```php
$cachePool = new \Drupal\PSRCache\CachePool();
$cacheItem = $cachePool->getItem('foo');
var_dump($cacheItem->get());
```

Setting an element:

```php
$cachePool = new \Drupal\PSRCache\CachePool();
$cacheItem->set(['foo' => 'bar']);
$cachePool->save($cacheItem);
```

Using the static cache proxy:

```php
$cachePool = new \Drupal\PSRCache\CachePool();
$staticCache = new \Drupal\PSRCache\StaticCacheProxy($cachePool);
$cacheItem = $staticCache->getItem('foo');
var_dump($cacheItem->get());
```
