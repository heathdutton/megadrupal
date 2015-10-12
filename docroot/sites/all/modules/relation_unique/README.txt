The Relation Unique module extends the Relation module with another "property"
that constrains relations of a given type, much like the built-in properties
"unique" and "transitive".

Relation Unique adds "endpoint uniqueness" to the provided properties:
- For a non-directed relation, entity uniqueness ensures that any entity can
  only occur in one relation of the relation type. E.g. in a binary undirected
  relation "married to", entity uniqueness prevents polygamy.
- For a directed relation, there are two kinds of entity uniqueness:
  - Source uniqueness: If a relation type is source unique, any entity can only
    occur as source in exactly one relation of the type.
    Mathematically speaking, this turns the relation type into a function.
  - Target uniqueness: If a relation type is target unique, any entity can only
    occur as target / among the targets in exactly one relation of the type.
    Mathematically speaking, for binary directed relations, this turns the 
    inverse of the relation type into a function.
    
This property is easily enforced (in one direction) with the *reference fields,
(by setting its cardinality to 1). In the other direction, it was basically 
impossible to guarantee this kind of uniqueness. Relation module does not 
currently give you a setting to enforce this constraint.

One particular use case for this is tree structures: In a tree, any child 
can have only one parent. This could easily be guaranteed, by having one
directed is "is mother of" relation which has the mother node as source, the
children as target, and turning on target uniqueness.

--------------------------------------------------------------------------------
- REQUIREMENTS                                                                 -
--------------------------------------------------------------------------------

Presently, Relation Unique only works with the SQL field storage engine (which
is the default). If you don't know what that means, you are probably using the
SQL engine.

--------------------------------------------------------------------------------
- INSTALLATION / USAGE                                                         -
--------------------------------------------------------------------------------

Install and enable as usual. You can then go to the edit page of an existing 
relation type (or create a new one) and set it entity unique in the "Advanced"
fieldset of the form.
