# delete all lom resources witch are marked as deleted
DELETE FROM archibald_lom_stats WHERE lom_id IN (
	SELECT l.lom_id
	  FROM archibald_lom l
	  WHERE l.deleted=1 AND l.save_time = (
		SELECT la.save_time
		  FROM archibald_lom la
		  WHERE l.lom_id=la.lom_id
		  ORDER BY la.save_time DESC LIMIT 1));

DELETE FROM archibald_lom WHERE lom_id IN (
	SELECT l.lom_id
	  FROM archibald_lom l
	  WHERE l.deleted=1 AND l.save_time = (
		SELECT la.save_time
		  FROM archibald_lom la
		  WHERE l.lom_id=la.lom_id
		  ORDER BY la.save_time DESC LIMIT 1));

DELETE FROM archibald_meta_metadata WHERE meta_metadata_id IN (
	SELECT m.meta_metadata_id
	  FROM archibald_meta_metadata_contributes m
	  LEFT JOIN archibald_lom l ON (m.meta_metadata_id=l.meta_metadata_id)
	  WHERE l.meta_metadata_id IS NULL);

DELETE FROM archibald_meta_metadata_identifier WHERE meta_metadata_id IN (
	SELECT m.meta_metadata_id
	  FROM archibald_meta_metadata_contributes m
	  LEFT JOIN archibald_lom l ON (m.meta_metadata_id=l.meta_metadata_id)
	  WHERE l.meta_metadata_id IS NULL);


DELETE FROM archibald_meta_metadata_contributes WHERE meta_metadata_id IN (
	SELECT m.meta_metadata_id
	  FROM archibald_meta_metadata_contributes m
	  LEFT JOIN archibald_lom l ON (m.meta_metadata_id=l.meta_metadata_id)
	  WHERE l.meta_metadata_id IS NULL);

DELETE FROM archibald_contribute_entity WHERE contribute_id NOT IN (
	SELECT c.contribute_id
	  FROM archibald_lifecycle_contributes c
	  INNER JOIN archibald_lom l ON (c.lifecycle_id=l.lifecycle_id)
)

DELETE FROM archibald_lifecycle_contributes WHERE lifecycle_id IN (
	SELECT m.lifecycle_id
	  FROM archibald_lifecycle m
	  LEFT JOIN archibald_lom l ON (m.lifecycle_id=l.lifecycle_id)
	  WHERE l.lifecycle_id IS NULL);

DELETE FROM archibald_lifecycle WHERE lifecycle_id IN (
	SELECT m.lifecycle_id
	  FROM archibald_lifecycle m
	  LEFT JOIN archibald_lom l ON (m.lifecycle_id=l.lifecycle_id)
	  WHERE l.lifecycle_id IS NULL);

DELETE FROM archibald_lifecycle WHERE lifecycle_id IN (
	SELECT m.lifecycle_id
	  FROM archibald_lifecycle m
	  LEFT JOIN archibald_lom l ON (m.lifecycle_id=l.lifecycle_id)
	  WHERE l.lifecycle_id IS NULL);


# Delete all not used vCards
DELETE FROM archibald_contributors WHERE contributor_id NOT IN (
	SELECT c.entity_id
	  FROM archibald_contribute_entity c
	UNION
	SELECT v.field_user_vcard_value AS entity_id
	 FROM field_data_field_user_vcard v)