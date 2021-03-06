// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

// #SchematycTODO : Store stream ids rather than names in order to optimize filtering.
// #SchematycTODO : Display separate report when opening editor rather than copying recorded messages to log?
// #SchematycTODO : Should we allow log widgets to output messages when tabbed (i.e. not visible but not closed)?

#pragma once

#include "QScrollableBox.h"

#include <QTextBrowser>
#include <QWidget>
#include <Schematyc/Services/LogStreamName.h>
#include <Schematyc/SerializationUtils/SerializationUtils.h>
#include <Schematyc/Utils/ScopedConnection.h>

class QBoxLayout;
class QLineEdit;
class QParentWndWidget;
class QPlainTextEdit;
class QAdvancedPropertyTree;
class QPushButton;
class QSplitter;

namespace Schematyc {

typedef std::vector<SLogStreamName> LogStreams;

struct SLogSettings
{
	SLogSettings();

	void Serialize(Serialization::IArchive& archive);

	string     entity;
	LogStreams streams;
	bool       bShowComments;
	bool       bShowWarnings;
	bool       bShowErrors;
	bool       bShowEntity;
	bool       bShowOrigin;
};

class CLogSettingsWidget : public QScrollableBox
{
	Q_OBJECT

public:
	CLogSettingsWidget(SLogSettings& settings);

	const SLogSettings& GetSettings() const;

//protected Q_SLOTS:
//	void OnAttachToSelectedEntityButtonClicked();

protected:
	void showEvent(QShowEvent* pEvent);

private:
	SLogSettings&          m_settings;
	QBoxLayout*            m_pLayout;
	QPushButton*           m_pAttachToSelectedEntityButton;
	QAdvancedPropertyTree* m_pPropertyTree;
};

class CLogWidget : public QWidget
{
	Q_OBJECT

	typedef std::vector<QString> PendingMessages;

public:
	CLogWidget(const SLogSettings& settings);

	void InitLayout();
	void Clear();

protected:
	void timerEvent(QTimerEvent* pTimerEvent);

	void OnLinkClicked(const QUrl& link);

private:
	EVisitStatus VisitRecordedLogMessage(const SLogMessageData& logMessageData);
	void         OnLogMessage(const SLogMessageData& logMessageData);
	void         WriteLogMessageToOutput(const SLogMessageData& logMessageData);

private:

	QBoxLayout*         m_pMainLayout;
	const SLogSettings& m_settings;
	QTextBrowser*       m_pOutput;

	PendingMessages     m_pendingMessages;
	CConnectionScope    m_connectionScope;
};
} // Schematyc
