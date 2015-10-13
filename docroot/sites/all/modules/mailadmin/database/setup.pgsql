-- Set up functions, triggers and views for PostgreSQL.
--
-- These provide the secure interface used by the mail servers and makes
-- sure that the data in our tables is reasonably sane.
--
-- Much inspiration from depesz:
-- http://www.depesz.com/index.php/tag/exim/

-- View that provides a list of all the domains we currently provide
-- e-mail accounts for.
CREATE OR REPLACE VIEW mailadmin_local_domains AS
  SELECT DISTINCT(domain_name) FROM mailadmin_mailboxes
  UNION
  SELECT DISTINCT(domain_name) FROM mailadmin_forwards
  UNION    
  SELECT source_domain_name FROM mailadmin_domain_aliases;

-- Function called by Exim/Postfix to determine if a domain is local,
-- ie. if our server should handle its e-mail.
CREATE OR REPLACE FUNCTION mailadmin_is_local_domain(in_domain TEXT) RETURNS TEXT as $BODY$
  DECLARE
    found_name TEXT;
  BEGIN
    SELECT domain_name INTO found_name 
      FROM mailadmin_local_domains 
      WHERE domain_name = trim(both FROM lower(in_domain));

    IF NOT FOUND THEN
      RETURN NULL;
    END IF;

    RETURN in_domain;
  END;
$BODY$ LANGUAGE plpgsql SECURITY DEFINER;

-- Get all alias (forward) destination for an address or an alias created
-- through a domain alias.
CREATE OR REPLACE FUNCTION mailadmin_alias_destinations(in_local_part TEXT, in_domain TEXT) RETURNS SETOF TEXT AS $BODY$
  DECLARE
    use_local_part text := trim(both FROM lower(in_local_part));
    use_domain text := trim(both FROM lower(in_domain));
    temprec record;
  BEGIN
    -- Try to get a domain alias
    SELECT target_domain_name FROM mailadmin_domain_aliases into temprec
    WHERE source_domain_name = use_domain;
    IF FOUND THEN
      RETURN NEXT use_local_part || '@' || temprec.target_domain_name;
    END IF;

    -- Try to get a forward alias.
    FOR temprec IN
      SELECT * FROM mailadmin_forwards
      WHERE local_part = use_local_part AND domain_name = use_domain
    LOOP
      RETURN NEXT temprec.destination;
    END LOOP;

    RETURN;
  END;
$BODY$ LANGUAGE plpgsql SECURITY DEFINER;

-- Get all alias (forward) destination for an address.
CREATE OR REPLACE FUNCTION mailadmin_get_mailbox(in_local_part TEXT, in_domain TEXT) RETURNS mailadmin_mailboxes AS $BODY$
  DECLARE
    use_local_part text := trim(both FROM lower(in_local_part));
    use_domain text := trim(both FROM lower(in_domain));
    temprec mailadmin_mailboxes%ROWTYPE;
  BEGIN
    SELECT * FROM mailadmin_mailboxes INTO temprec
      WHERE local_part = use_local_part AND domain_name = use_domain;

    IF NOT FOUND THEN
      RETURN NULL;
    END IF;

    RETURN temprec;
  END;
$BODY$ LANGUAGE plpgsql SECURITY DEFINER;

-- Function to get the home directory for a user.
CREATE OR REPLACE FUNCTION mailadmin_account_homedir(in_local_part TEXT, in_domain TEXT, in_prefix TEXT) RETURNS TEXT as $BODY$
  DECLARE
    use_local_part text := trim(both FROM lower(in_local_part));
    use_domain text := trim(both FROM lower(in_domain));
    temprecord record;
  BEGIN
    SELECT mailbox_id, home INTO temprecord
      FROM mailadmin_mailboxes
      WHERE domain_name = use_domain AND local_part = use_local_part;

    IF NOT FOUND THEN
      RETURN NULL;
    END IF;

    IF temprecord.home IS NULL THEN
      RETURN in_prefix || '/' || use_domain || '/' || use_local_part;
    END IF;

    RETURN in_prefix || '/' || temprecord.home;
  END;
$BODY$ LANGUAGE plpgsql SECURITY DEFINER;

-- Function to check if we accept local delivery for a specific e-mail address.
CREATE OR REPLACE FUNCTION mailadmin_is_local_user(in_local_part TEXT, in_domain TEXT) RETURNS TEXT as $BODY$
  DECLARE
    use_local_part text := trim(both FROM lower(in_local_part));
    use_domain text := trim(both FROM lower(in_domain));
    tempint INT4;
  BEGIN
    SELECT mailbox_id INTO tempint 
      FROM mailadmin_mailboxes
      WHERE domain_name = use_domain AND local_part = use_local_part;

    IF NOT FOUND THEN
      RETURN NULL;
    END IF;

    RETURN in_local_part || '@' || in_domain;
  END;
$BODY$ LANGUAGE plpgsql SECURITY DEFINER;

-- Function to get the maildir for a user.
CREATE OR REPLACE FUNCTION mailadmin_account_maildir(in_local_part TEXT, in_domain TEXT, in_prefix TEXT) RETURNS TEXT as $BODY$
  DECLARE
    homedir text;
  BEGIN
    SELECT mailadmin_account_homedir(in_local_part, in_domain, in_prefix) INTO homedir;

    IF NOT FOUND OR homedir IS NULL THEN
      RETURN NULL;
    END IF;

    RETURN homedir || '/maildir';
  END;
$BODY$ LANGUAGE plpgsql SECURITY DEFINER;

-- Function to clean up domain names when inserted into the database.
CREATE OR REPLACE FUNCTION mailadmin_clean_domain_name() RETURNS TRIGGER AS $BODY$
  DECLARE
  BEGIN
    NEW.local_part := trim(both FROM lower(NEW.local_part));
    NEW.domain_name := trim(both FROM lower(NEW.domain_name));
    RETURN NEW;
  END;
$BODY$ LANGUAGE plpgsql;

-- Trigger the cleanup everytime a new entry is added.
DROP TRIGGER IF EXISTS mailadmin_mailboxes_clean_domain_name ON mailadmin_mailboxes;
DROP TRIGGER IF EXISTS clean_domain_name ON mailadmin_mailboxes;
CREATE TRIGGER clean_domain_name
  BEFORE INSERT OR UPDATE ON mailadmin_mailboxes 
  FOR EACH ROW EXECUTE PROCEDURE mailadmin_clean_domain_name();
DROP TRIGGER IF EXISTS mailadmin_forwards_clean_domain_name ON mailadmin_forwards;
DROP TRIGGER IF EXISTS clean_domain_name ON mailadmin_forwards;
CREATE TRIGGER clean_domain_name
  BEFORE INSERT OR UPDATE ON mailadmin_forwards 
  FOR EACH ROW EXECUTE PROCEDURE mailadmin_clean_domain_name();

