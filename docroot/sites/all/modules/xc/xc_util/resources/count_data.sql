SELECT
(SELECT count(*) FROM xc_entity_properties) AS 'entity',
(SELECT count(*) FROM xc_sql_metadata) AS 'meta',
(SELECT count(*) FROM xc_entity_relationships) AS 'rel',
(SELECT count(*) FROM xc_oaiharvester_bridge_header) AS 'header',
(SELECT count(*) FROM xc_oaiharvester_bridge_set) AS 'set',
(SELECT count(*) FROM node) AS 'node',
(SELECT count(*) FROM node_revision) AS 'rev',
(SELECT count(*) FROM node_comment_statistics) AS 'comment';
