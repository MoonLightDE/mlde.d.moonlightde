/* 
 * File:   Node.h
 * Author: alexis
 *
 * Created on 3 de marzo de 2015, 10:55
 */

#ifndef NODE_H
#define	NODE_H

#include <QString>
#include <QVariant>
#include <QList>


namespace model_filesystem {

    class Node {
    public:

        enum Property {
            NAME = 0,
            ICON_NAME,
            MIMETYPE,
            SIZE,
            STORED_SIZE
        };
        

        typedef QMap<Node::Property, QVariant> PropertyMap;

        Node(QString uri) : m_Uri(uri) {
        }

        Node(const Node& orig) {
            m_Uri = orig.m_Uri;
        }

        virtual ~Node() {
        }

        // Dummy implementations

        virtual Node parent() {
            return Node(QString());
        }

        virtual QList<Node> children() {
            return QList<Node>();
        }

        /**
         * Get node property. This method will use cache data, if available to
         * update the cache data use the method <code>queryProperty</code> else 
         * it will call <code>queryProperty</code>.
         * @param property
         * @return 
         */
        virtual QVariant property(Property property) {
            return QVariant();
        }

        /**
         * Query the file system for node properties, this method updates the
         * cache, see <code>property</code> to use cached data instead.
         * @param property
         * @return 
         */
        virtual QVariant queryProperty(Property property) {
            return QVariant();
        }

        /**
         * Sets a value for the given property.
         * @param property
         * @param value
         */
        virtual void setProperty(Property property, QVariant value) {
        }

        /**
         * Get node properties. This method will use cache data, if available to
         * update the cache data use the method <code>queryProperties</code> else 
         * it will call <code>queryProperties</code>.
         * @param properties
         * @return 
         */
        virtual PropertyMap properties(QList<Property> properties) {
            return PropertyMap();
        }

        /**
         * Query the file system for node properties, this method updates the
         * cache, see <code>property</code> to use cached data instead.
         * @param properties
         * @return 
         */
        virtual PropertyMap queryProperties(QList<Property> properties) {
            return PropertyMap();
        }

        virtual void setProperties(PropertyMap properties) {
        }

        virtual bool isValid() {
            return m_Uri.isNull();
        }

        void setUri(const QString &uri) {
            m_Uri = m_Uri;
        }

        QString getUri() {
            return m_Uri;
        }

    private:
        QString m_Uri;
    };


}

#endif	/* NODE_H */

