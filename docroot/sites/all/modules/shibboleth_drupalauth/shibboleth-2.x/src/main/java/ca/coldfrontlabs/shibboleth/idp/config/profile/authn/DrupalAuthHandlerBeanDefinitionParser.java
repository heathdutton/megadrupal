package ca.coldfrontlabs.shibboleth.idp.config.profile.authn;
import javax.xml.namespace.QName;
import org.opensaml.xml.util.DatatypeHelper;
 
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
 
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
 
import org.w3c.dom.Element;
 
import ca.coldfrontlabs.shibboleth.idp.config.profile.ProfileHandlerDrupalAuthHandler;
import edu.internet2.middleware.shibboleth.idp.config.profile.authn.AbstractLoginHandlerBeanDefinitionParser;
 
public class DrupalAuthHandlerBeanDefinitionParser extends AbstractLoginHandlerBeanDefinitionParser {
 
    /** Schema type. */
    public static final QName SCHEMA_TYPE = new QName(ProfileHandlerDrupalAuthHandler.NAMESPACE, "DrupalAuth");
 
    /** Class logger. */
    private final Logger log = LoggerFactory.getLogger(DrupalAuthHandlerBeanDefinitionParser.class);
 
    /** {@inheritDoc} */
    protected Class getBeanClass(Element element) {
        return DrupalAuthHandlerFactoryBean.class;
    }
 
    /** {@inheritDoc} */
    protected void doParse(Element config, BeanDefinitionBuilder builder) {
        super.doParse(config, builder);
 
        builder.addPropertyValue("authCookieName", DatatypeHelper.safeTrim(config.getAttributeNS(null,"authCookieName")));
        builder.addPropertyValue("authValidationEndpoint", DatatypeHelper.safeTrim(config.getAttributeNS(null,"authValidationEndpoint")));
        builder.addPropertyValue("drupalLoginURL", DatatypeHelper.safeTrim(config.getAttributeNS(null,"drupalLoginURL")));
        builder.addPropertyValue("authenticationServletURL", DatatypeHelper.safeTrim(config.getAttributeNS(null,"authenticationServletURL")));
        builder.addPropertyValue("xforwardedHeader", DatatypeHelper.safeTrim(config.getAttributeNS(null,"xforwardedHeader")));
        builder.addPropertyValue("validateSessionIP", DatatypeHelper.safeTrim(config.getAttributeNS(null,"validateSessionIP")));
    }
}
