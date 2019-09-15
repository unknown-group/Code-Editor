#include "complete.h"

CompleteListWidget::CompleteListWidget(QWidget *parent):QListWidget(parent)
{
  p=(QPlainTextEdit*)parent;
  backgroundColor=Qt::lightGray;//.setRgb(34,39,49);
  highlightColor.setRgb(22,165,248);
  QPalette palette=this->palette();
  palette.setColor(QPalette::Active,QPalette::Highlight,highlightColor);
  palette.setColor(QPalette::Inactive,QPalette::Highlight,highlightColor);
  palette.setColor(QPalette::Active, QPalette::Base,backgroundColor);
  palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
  palette.setColor(QPalette::Text,Qt::white);
  this->setPalette(palette);
}

void CompleteListWidget::keyPressEvent(QKeyEvent *event){
  if(event->key()==16777235||event->key()==16777237){
      QListWidget::keyPressEvent(event);
    }else{
      QApplication::sendEvent(p,event);
      p->setFocus();
    }
}
int CompleteListWidget::ldistance(const std::string source, const std::string target){
  //step 1

          int n = source.length();
          int m = target.length();
          if (m == 0) return n;
          if (n == 0) return m;
          //Construct a matrix
          typedef vector< vector<int> >  Tmatrix;
          Tmatrix matrix(n + 1);
          for (int i = 0; i <= n; i++)  matrix[i].resize(m + 1);
          //step 2 Initialize
          for (int i = 1; i <= n; i++) matrix[i][0] = i;
          for (int i = 1; i <= m; i++) matrix[0][i] = i;
          //step 3
          for (int i = 1; i <= n; i++)
          {
                  const char si = source[i - 1];
                  //step 4
                  for (int j = 1; j <= m; j++)
                  {
                          const char dj = target[j - 1];
                          //step 5
                          int cost;
                          if (si == dj){
                                  cost = 0;
                          }
                          else{
                                  cost = 1;
                          }
                          //step 6
                          const int above = matrix[i - 1][j] + 1;
                          const int left = matrix[i][j - 1] + 1;
                          const int diag = matrix[i - 1][j - 1] + cost;
                          matrix[i][j] = min(above, min(left, diag));
                  }
          }//step7
          return matrix[n][m];
}

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{

    //初始化补全列表
    setUpCompleteList();
    completeWidget= new CompleteListWidget(this);
    completeWidget->hide();
    completeWidget->setMaximumHeight(fontMetrics().height()*5);
    completeState=CompleteState::Hide;
}

void CodeEditor::keyPressEvent(QKeyEvent *event){
  //qDebug()<<event->key();
  if(event->modifiers()==Qt::ShiftModifier&&event->key()==40){
      this->insertPlainText(tr("()"));
      this->moveCursor(QTextCursor::PreviousCharacter);
    }
  else if(event->modifiers()==Qt::ShiftModifier&&event->key()==34){
      this->insertPlainText(tr("\"\""));
      this->moveCursor(QTextCursor::PreviousCharacter);
    }
  else if(event->key()==16777235&&completeState==CompleteState::Showing){
      if(completeWidget->currentRow()>0)
        completeWidget->setCurrentRow(completeWidget->currentRow()-1);
    }
  else if(event->key()==16777237&&(completeState==CompleteState::Showing)){
      if(completeWidget->currentRow()<completeWidget->count()-1)
        completeWidget->setCurrentRow(completeWidget->currentRow()+1);
    }
  else if(event->key()==Qt::Key_Return&&(completeState==CompleteState::Showing)){
      QString insertText=completeWidget->currentItem()->text();
      QString word=this->getWordOfCursor();
      completeState=CompleteState::Ignore;
      for(int i=0;i<word.count();++i)
        this->textCursor().deletePreviousChar();
      this->insertPlainText(insertText);
      if(insertText.contains(tr("#include")))
        this->moveCursor(QTextCursor::PreviousCharacter);
      completeState=CompleteState::Hide;
      completeWidget->hide();
    }
  else if(event->key()==Qt::Key_Return){//回车下行层级自动缩进功能
      //获得本行的文本
      QString temp=this->document()->findBlockByLineNumber(this->textCursor().blockNumber()).text();
      QPlainTextEdit::keyPressEvent(event);
      if(temp.count()<=0)return;
      //输出回车那一行的前距
      foreach(const QChar &c,temp){
          if(c.isSpace())this->insertPlainText(c);
          else break;
        }
      //如果是for() while() switch() if()则缩进一个tab,一种粗略地做法可能会出错
      if(temp.at(temp.count()-1)==')'&&(temp.contains(tr("for("))||temp.contains(tr("while("))
                                        ||temp.contains(tr("switch("))||temp.contains(tr("if("))))
          this->insertPlainText(tr("\t"));
      //如果是{ 则缩进并补}
      if(temp.at(temp.count()-1)=='{'){
          this->insertPlainText(tr("\t"));
          QTextCursor cursor=this->textCursor();
          int pos=this->textCursor().position();
          this->insertPlainText(tr("\n"));
          foreach(const QChar &c,temp){
              if(c.isSpace())this->insertPlainText(c);
              else break;
            }
          this->insertPlainText(tr("}"));
          cursor.setPosition(pos);
          this->setTextCursor(cursor);//返回中间一行
        }
    }//
  else if(event->key()==Qt::Key_Backspace){
      switch(this->document()->characterAt(this->textCursor().position()-1).toLatin1()){
        case '(':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position())==')'){
              this->textCursor().deleteChar();
            }break;
        case '\"':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position())=='\"'){
              this->textCursor().deleteChar();
            }break;
        case '<':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position())=='>'){
              this->textCursor().deleteChar();
            }break;
        default:
          QPlainTextEdit::keyPressEvent(event);
        }
    }
  else{
    QPlainTextEdit::keyPressEvent(event);
    //qDebug()<<event->key();
    }
}

void CodeEditor::setUpCompleteList(){
  completeList<< "char" << "class" << "const"
              << "double" << "enum" << "explicit"
              << "friend" << "inline" << "int"
              << "long" << "namespace" << "operator"
              << "private" << "protected" << "public"
              << "short" << "signals" << "signed"
              << "slots" << "static" << "struct"
              << "template" << "typedef" << "typename"
              << "union" << "unsigned" << "virtual"
              << "void" << "volatile" << "bool"<<"using"<<"constexpr"
              <<"sizeof"<<"if"<<"for"<<"foreach"<<"while"<<"do"<<"case"
              <<"break"<<"continue"<<"template"<<"delete"<<"new"
              <<"default"<<"try"<<"return"<<"throw"<<"catch"<<"goto"<<"else"
              <<"extren"<<"this"<<"switch"<<"#include <>"<<"#include \"\""<<"#define"<<"iostream";
}
//得到当前光标位置的字符串
QString CodeEditor::getWordOfCursor(){
  int pos=this->textCursor().position()-1;
  QVector<QChar> words;
  QString result;
  QChar ch=this->document()->characterAt(pos+1);
  if(ch.isDigit()||ch.isLetter()||ch==' ')return result;
  ch=this->document()->characterAt(pos);
  if(ch==' ')return result;
  while(ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#'){
      words.append(ch);
      pos--;
      ch=this->document()->characterAt(pos);
    }
  for(int i=words.size()-1;i>=0;i--)
    result+=words[i];
  return result;

}

void CodeEditor::showCompleteWidget(){
  if(completeState==CompleteState::Ignore)return;//忽略光标和文本变化的响应,避免陷入事件死循环和互相钳制
  completeWidget->hide();
  completeState=CompleteState::Hide;
  QString word=this->getWordOfCursor();
  completeWidget->clear();
  if(!word.isEmpty()){//光标所在单词是不是合法(能不能联想)
      int maxSize=0;
      QMap<QString,int> distance;
      vector<QString> itemList;
      foreach(const QString &temp,completeList){
          if(temp.contains(word)){
              //completeWidget->addItem(new QListWidgetItem(temp));
              itemList.push_back(temp);
              distance[temp]=CompleteListWidget::ldistance(temp.toStdString(),word.toStdString());
              if(temp.length()>maxSize)maxSize=temp.length();

            }
        }
      //有没有匹配的字符
      if(itemList.size()>0){//如果有的话
      sort(itemList.begin(),itemList.end(),[&](const QString &s1,const QString &s2)->bool{return distance[s1]<distance[s2]; });
      foreach(const QString& item,itemList){
          completeWidget->addItem(new QListWidgetItem(item));
        }

      int x=this->getCompleteWidgetX();
      int y=this->cursorRect().y()+fontMetrics().height();

      completeWidget->move(x,y);
      if(completeWidget->count()>5)completeWidget->setFixedHeight(fontMetrics().height()*6);
      else completeWidget->setFixedHeight(fontMetrics().height()*(completeWidget->count()+1));
      completeWidget->setFixedWidth((fontMetrics().width(QLatin1Char('9'))+6)*maxSize);
      completeWidget->show();
      completeState=CompleteState::Showing;
      completeWidget->setCurrentRow(0,QItemSelectionModel::Select);
        }
    }

}
int CodeEditor::getCompleteWidgetX(){
  QTextCursor cursor=this->textCursor();
  int pos=cursor.position()-1;
  int origianlPos=pos+1;
  QChar ch;
  ch=this->document()->characterAt(pos);
  while((ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#')&&pos>0){
      pos--;
      ch=this->document()->characterAt(pos);
    }
  pos++;
  completeState=CompleteState::Ignore;
  cursor.setPosition(pos);
  this->setTextCursor(cursor);
  int x=this->cursorRect().x()+2*fontMetrics().width(QLatin1Char('9'));
  cursor.setPosition(origianlPos);
  this->setTextCursor(cursor);
  completeState=CompleteState::Hide;
  return x;
}
