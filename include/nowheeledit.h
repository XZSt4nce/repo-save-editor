#pragma once

class NoWheelSpinBox final : public QSpinBox
{
public:
	explicit NoWheelSpinBox( QWidget* parent = nullptr ) : QSpinBox( parent ) {}

protected:
	void wheelEvent( QWheelEvent* event ) override
	{
		if ( QApplication::mouseButtons() != Qt::NoButton && hasFocus() )
			QSpinBox::wheelEvent( event );
		else
			event->ignore();
	}
};

class NoWheelDoubleSpinBox final : public QDoubleSpinBox
{
public:
	explicit NoWheelDoubleSpinBox( QWidget* parent = nullptr ) : QDoubleSpinBox( parent ) {}

protected:
	void wheelEvent( QWheelEvent* event ) override
	{
		if ( QApplication::mouseButtons() != Qt::NoButton && hasFocus() )
			QDoubleSpinBox::wheelEvent( event );
		else
			event->ignore();
	}
};

class NoWheelDateEdit final : public QDateEdit
{
public:
	explicit NoWheelDateEdit(QWidget* parent = nullptr) : QDateEdit(parent) {}

protected:
	void wheelEvent(QWheelEvent* event) override
	{
		if (QApplication::mouseButtons() != Qt::NoButton && hasFocus())
			QDateEdit::wheelEvent(event);
		else
			event->ignore();
	}
};
