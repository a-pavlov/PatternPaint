#ifndef PATTERNDITOR_H
#define PATTERNDITOR_H

#include <QWidget>
#include <QMetaType>

class QUndoStack;
class UndoCommand;
class AbstractInstrument;

#define DEFAULT_PATTERN_HEIGHT 60
#define DEFAULT_PATTERN_LENGTH 100

class PatternEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PatternEditor(QWidget *parent = 0);
    virtual ~PatternEditor();

    /// Initialize the pattern editor using a QImage as the new pattern
    /// @param newPattern New pattern to load
    /// @param scaled If true, scale the image to match the height of the previous pattern
    bool init(QImage newPattern, bool scaled = true);

    void setImage(const QImage& img) { pattern = img; }

    inline QUndoStack* getUndoStack() { return m_undoStack; }
    inline void setUndoStack(QUndoStack* stack) { m_undoStack = stack; }

    /// Get the image data for the current pattern
    /// @return QImage containing the current pattern
    QImage getPatternAsImage() const { return pattern; }
    QImage* getPattern() { return &pattern; }

    bool isEdited() const { return m_edited; }
    void setEdited(bool e) {
        m_edited = e;
        emit changed(m_edited);
    }

    // for compatibility with EasyPaint sources
    QColor getPrimaryColor() const { return toolColor; }
    int getPenSize() const { return toolSize; }

    /// Re-size the display grid and selector based on new widget geometery
    void resizeEvent(QResizeEvent * event);

    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent*);

    void leaveEvent(QEvent * event);

    /**
     * @brief pushUndoCommand
     * @param command - manually generated command for undo
     */
    void pushUndoCommand(UndoCommand *command);

    /**
     * @brief pushUndoCommand - self generated undo command
     */
    void pushUndoCommand();
    bool isPaint() const { return m_isPaint; }
    void setPaint(bool paint) { m_isPaint = paint; }

    AbstractInstrument* instrument() { return m_pi; }
    void update();
protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage pattern;        ///< The actual image
    QImage gridPattern;    ///< Holds the pre-rendered grid overlay
    QImage toolPreview;    ///< Holds a preview of the current tool

    float xScale;          ///< Number of pixels in the grid pattern per pattern pixel.
    float yScale;          ///< Number of pixels in the grid pattern per pattern pixel.

    QColor toolColor;      ///< Color of the current drawing tool (TODO: This should be a pointer to a tool)
    int toolSize;          ///< Size of the current drawing tool (TODO: This should be a pointer to a tool)

    int playbackRow;       ///< Row being played back
    QUndoStack* m_undoStack;
    bool m_isPaint;
    AbstractInstrument* m_pi;
    bool m_edited;

    /// Redraw the gridPattern to fit the current widget size.
    void updateGridSize();

    /// Update the screen, but only if we haven't done so in a while
    void lazyUpdate();
    void updateToolPreview(int x, int y);
signals:
    void changed(bool);
    void updated();
    void resized();
public slots:
    void setToolColor(QColor color);
    void setToolSize(int size);
    void setPlaybackRow(int row);
    void setInstrument(AbstractInstrument*);
};

Q_DECLARE_METATYPE(PatternEditor*)

#endif // PATTERNDITOR_H
