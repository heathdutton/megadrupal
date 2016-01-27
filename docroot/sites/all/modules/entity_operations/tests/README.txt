Entity Operations tests
=======================

Our tests come with two different entity types:

- entity_operations_test: for testing entities that use Entity API's system for
  entity forms.
- entity_operations_test_generic: for testing entities that use Entity
  Operations' generic entity form operation handlers.

Either module may be enabled to provide test entities. The first one,
entity_operations_test is recommended over the second, as it comes with admin UI
for the entities, since it uses Entity API.
