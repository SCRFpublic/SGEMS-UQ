/*
 * metricdommodel.h
 *
 *  Created on: Apr 26, 2011
 *      Author: aboucher
 */

#ifndef METRICDOMMODEL_H_
#define METRICDOMMODEL_H_

#include "dommodel.h"
#include "common.h"
#include "MetricDataManager.h"

#include <QtXml>
#include <QMimeData>
#include <QModelIndexList>

#include <string>

using namespace std;

class ITEM_MODEL_DECL MetricDomModel : public DomModel
{
    Q_OBJECT

public:

	MetricDomModel(QDomDocument document, QObject *parent = 0);

	virtual ~MetricDomModel();
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

	virtual QStringList mimeTypes() const;
	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;

public slots:
	void SaveToFile(const QModelIndex&, const QString& fileName);

private:
	void AddQDomForProperty(MetricDataManager* registrar, QDomDocument& doc, QDomElement& root, string& metric_name, string& grid_name, string& property_name);
};

#endif /* METRICDOMMODEL_H_ */
