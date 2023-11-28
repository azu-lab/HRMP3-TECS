require 'json'
require 'gtk3'

def parse_and_draw(editor, cr)
  # エディタの内容を取得
  editor_content = editor.buffer.text

  # 内容を解析
  data = parse_input_from_string(editor_content)

  # 解析結果を使用して描画
  width,height = draw_analysis(data, cr)

  return width, height
end

# エディタの内容を解析する関数
def parse_input_from_string(input_str)
  data = {}
  current_cls = nil
  current_som = nil

  # JSONファイルをパースしてハッシュに変換
  json_data = JSON.parse(input_str)

  # data[:cycle_time]にあたる値をJSON中の”SystemCycleTime”の値にする
  data[:cycle_time] = json_data["SystemCycleTime"]

  # data[current_som.to_sym]にあたる値をJSON中の”Name”の値にする
  json_data["SOM"].each do |som|
    current_som = som["name"]
    data[current_som.to_sym] = {}

    # data[current_som.to_sym][current_cls.to_sym]にあたる値をJSON中の”CLS_ALL_PRC1”や”CLS_PRC1”の値にする
    som.each do |key, value|
      if key.start_with?('CLS_')
        current_cls = key
        data[current_som.to_sym][current_cls.to_sym] = {}

        # data[current_som.to_sym][current_cls.to_sym][dom.strip.to_sym]にあたる値をJSON中の”rUserDomain1”などの値にする
        value.each do |dom, val|
          data[current_som.to_sym][current_cls.to_sym][dom.to_sym] = val
        end
      end
    end
  end

  data
end


def draw_analysis(data, cr)
  # 色の設定
  colors = [
    [255.0, 75.0, 0.0],
    [77.0, 196.0, 255.0],
    [255.0, 241.0, 0.0],
    [199.0, 217.0, 230.0],
    [246.0, 170.0, 0.0]
  ]

  # その他の必要な変数の初期化
  dom_colors_hash = {}
  cls_hash = {}
  som_size = 0
  cls_size = 0
  dom_size = 0

  # カウントしたclsの数を格納するハッシュマップ
  cls_counts_per_som = {}

  # サイズと色のハッシュマップを計算
  data.keys.each do |som|
    next if som == :cycle_time
    som_size += 1
    # 各SOMのclsの数をカウント
    cls_count = 0
    data[som].keys.each do |cls|
      next if cls == :CLS_ALL
      cls_count += 1
      data[som][cls].keys.each do |dom|
        unless dom_colors_hash.key?(dom)
          dom_colors_hash[dom] = colors[dom_size % colors.length]
          dom_size += 1
        end
      end
    end

    # カウントしたclsの数をハッシュマップに格納
    cls_counts_per_som[som] = cls_count
  end

  # 最大のclsの数を求める
  cls_size = cls_counts_per_som.values.max

  # 描画領域のサイズを設定
  sct_bs_x = 600
  sct_bs_y = 30
  sct_bs_s_x = 100
  sct_bs_s_y = 60
  line_length = 40

  # 最終的な横幅と高さ
  width = 0
  height = 0

  # 全体の背景を白色に設定
  cr.set_source_rgb(1, 1, 1)
  cr.paint

  x_offset = sct_bs_s_x 
  y_offset = 0

  # SOMごとの描画
  som_count = 0
  data.keys.each do |som|
    next if som == :cycle_time

    # SOM名のテキスト描画
    cr.set_source_rgb(0, 0, 0)
    cr.move_to(x_offset + 300, sct_bs_s_y - 30)
    cr.show_text("#{som}")

    # SOMの四角を描画
    left_som_x_line = x_offset - 10
    rigth_som_x_line = x_offset + sct_bs_x + 10
    upper_som_y_line = sct_bs_s_y - 15
    under_som_y_line = sct_bs_s_y + (sct_bs_y + line_length + 30) * cls_size - 10
    # SOMの上横線の描画
    cr.set_dash([5, 5])
    cr.set_line_width(2)
    cr.move_to(10, upper_som_y_line)
    cr.line_to(10 + rigth_som_x_line, upper_som_y_line)
    cr.stroke
    # SOMの下横線の描画
    cr.set_line_width(2)
    cr.move_to(10, under_som_y_line)
    cr.line_to(10 + rigth_som_x_line, under_som_y_line)
    cr.stroke
    cr.set_dash([])
    # SOMの左縦線の描画
    cr.set_dash([10, 10])
    cr.set_line_width(2)
    cr.move_to(left_som_x_line, 10)
    cr.line_to(left_som_x_line, under_som_y_line + 20)
    cr.stroke
    # SOMの右縦線の描画
    cr.set_line_width(2)
    cr.move_to(rigth_som_x_line, 10)
    cr.line_to(rigth_som_x_line, under_som_y_line + 20)
    cr.stroke
    cr.set_dash([])

    width = rigth_som_x_line
    height = under_som_y_line


    # クラスごとのDOMの描画
    data[som].keys.each do |cls|
      # システムサイクルタイムの四角を描画
      cr.set_source_rgb(0, 0, 0)
      cr.rectangle(x_offset, sct_bs_s_y + y_offset, sct_bs_x, sct_bs_y)
      cr.stroke
      # システムサイクルタイムのテキスト描画
      cr.move_to(x_offset + 300, sct_bs_s_y + y_offset + 20)
      cr.show_text("#{data[:cycle_time]}")
      if cls == :CLS_ALL
        # クラス名のテキスト描画
        cr.move_to(x_offset - 70, (sct_bs_s_y + under_som_y_line)/2)
        cr.show_text("#{cls}")

        upper_dom_y_line = sct_bs_s_y + sct_bs_y
        under_dom_y_line = sct_bs_s_y + sct_bs_y + 100 * (cls_size-1) + line_length
        # DOMの左縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset, upper_dom_y_line)
        cr.line_to(x_offset, under_dom_y_line)
        cr.stroke
        # DOMの右縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + sct_bs_x, upper_dom_y_line)
        cr.line_to(x_offset + sct_bs_x, under_dom_y_line)
        cr.stroke
        # DOMの下横線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset, under_dom_y_line)
        cr.line_to(x_offset + sct_bs_x, under_dom_y_line)
        cr.stroke
            
        # クラスごとのDOMの描画
        x_dom_offset = x_offset
        data[som][cls].keys.each do |dom|
          x_ratio = data[som][cls][dom].to_f / data[:cycle_time].to_f
          x_add_offset = x_ratio * sct_bs_x

          # 背景色の四角形を描画
          cr.set_source_rgb(dom_colors_hash[dom][0]/255.0, dom_colors_hash[dom][1]/255.0, dom_colors_hash[dom][2]/255.0)
          cr.rectangle(x_dom_offset + 1, sct_bs_s_y + sct_bs_y + 1 + y_offset, x_add_offset, under_dom_y_line - sct_bs_s_y - sct_bs_y - 2 )
          cr.fill

          # DOM名と時間のテキスト描画
          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10), sct_bs_s_y + y_offset + line_length + 10)
          cr.show_text("#{dom}")

          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10), sct_bs_s_y + y_offset + line_length + 20)
          cr.show_text("#{data[som][cls][dom]}")
                
          x_dom_offset += x_add_offset

          # 縦線の描画
          cr.set_line_width(2)
          cr.move_to(x_dom_offset, sct_bs_s_y + sct_bs_y + y_offset)
          cr.line_to(x_dom_offset, sct_bs_s_y + sct_bs_y + y_offset + under_dom_y_line - sct_bs_s_y - sct_bs_y)
          cr.stroke
        end # ALL CLS DOM END
      elsif 
        # クラス名のテキスト描画
        cr.move_to(x_offset - 70, sct_bs_s_y + sct_bs_y + y_offset + 10)
        cr.show_text("#{cls}")

        upper_dom_y_line = sct_bs_s_y + sct_bs_y + y_offset
        # DOMの左縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset, upper_dom_y_line)
        cr.line_to(x_offset, upper_dom_y_line + line_length)
        cr.stroke
        # DOMの右縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + sct_bs_x, upper_dom_y_line)
        cr.line_to(x_offset + sct_bs_x, upper_dom_y_line + line_length)
        cr.stroke
        # DOMの下横線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset, upper_dom_y_line + line_length)
        cr.line_to(x_offset + sct_bs_x, upper_dom_y_line + line_length)
        cr.stroke

        # クラスごとのDOMの描画
        x_dom_offset = x_offset
        data[som][cls].keys.each do |dom|
          x_ratio = data[som][cls][dom].to_f / data[:cycle_time].to_f
          x_add_offset = x_ratio * sct_bs_x

          # 背景色の四角形を描画
          cr.set_source_rgb(dom_colors_hash[dom][0]/255.0, dom_colors_hash[dom][1]/255.0, dom_colors_hash[dom][2]/255.0)
          cr.rectangle(x_dom_offset + 1, sct_bs_s_y + sct_bs_y + 1 + y_offset, x_add_offset, line_length - 2 )
          cr.fill

          # DOM名と時間のテキスト描画
          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10), sct_bs_s_y + y_offset + line_length + 10)
          cr.show_text("#{dom}")

          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10), sct_bs_s_y + y_offset + line_length + 20)
          cr.show_text("#{data[som][cls][dom]}")
          
          x_dom_offset += x_add_offset

          # 縦線の描画
          cr.set_line_width(2)
          cr.move_to(x_dom_offset, sct_bs_s_y + sct_bs_y + y_offset)
          cr.line_to(x_dom_offset, sct_bs_s_y + sct_bs_y + y_offset + line_length)
          cr.stroke
        end # CLS DOM END
      end # IF END
      x_dom_offset = x_offset
      y_offset += 100
    end # CLS END
    som_count += 1
    x_offset = sct_bs_s_x * (som_count + 1) + sct_bs_x * som_count + 10
    y_offset = 0
  end # SOM END

  return width, height
end

# テキストを.jsonファイルとして保存するためのメソッド
def save_json(editor)
  dialog = Gtk::FileChooserDialog.new(
    title: "Save JSON File",
    action: Gtk::FileChooserAction::SAVE,
    buttons: [
      [Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL],
      [Gtk::Stock::SAVE, Gtk::ResponseType::ACCEPT]
    ]
  )

  filter = Gtk::FileFilter.new
  filter.name = "JSON files"
  filter.add_pattern("*.json")
  dialog.add_filter(filter)

  if dialog.run == Gtk::ResponseType::ACCEPT
    filename = dialog.filename
    File.open(filename, 'w') do |file|
      file.write(editor.buffer.text)
      json_data = JSON.parse(editor.buffer.text)  # JSONをパースしてハッシュに変換
      generate_timewindow_cfg(filename, json_data)  # ハッシュを引数として渡す
    end
  end

  dialog.destroy
end

def generate_timewindow_cfg(output_filename, json_data)
  File.open('Timewindow.cfg', 'w') do |file|
    file.puts "DEF_SCY({ #{json_data['SystemCycleTime']} });"
    
    json_data['SOM'].each do |som|
      file.puts "\nCRE_SOM( #{som['name']}, { TA_NULL, #{som['next']} });"
      
      som.each do |key, value|
        next if key == 'name' || key == 'next'
        
        file.puts "CLASS( #{key} ) {"
        value.each_with_index do |(domain, time), index|
          file.puts "\tATT_TWD({ #{domain}, #{som['name']} , #{index + 1}, #{time} });"
        end
        file.puts '}'
      end
    end
  end
end

# JSONを読み込むメソッド
def load_json(editor)
  dialog = Gtk::FileChooserDialog.new(
    title: "Open JSON File",
    action: Gtk::FileChooserAction::OPEN,
    buttons: [
      [Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL],
      [Gtk::Stock::OPEN, Gtk::ResponseType::ACCEPT]
    ]
  )

  filter = Gtk::FileFilter.new
  filter.name = "JSON files"
  filter.add_pattern("*.json")
  dialog.add_filter(filter)

  if dialog.run == Gtk::ResponseType::ACCEPT
    filename = dialog.filename
    file_content = File.read(filename)
    editor.buffer.text = file_content
  end
  dialog.destroy
end

# テキストを読み込むメソッド
def load_cdl(editor)
  dialog = Gtk::FileChooserDialog.new(
    title: "Open JSON File",
    action: Gtk::FileChooserAction::OPEN,
    buttons: [
      [Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL],
      [Gtk::Stock::OPEN, Gtk::ResponseType::ACCEPT]
    ]
  )

  filter = Gtk::FileFilter.new
  filter.name = "JSON files"
  filter.add_pattern("*.json")
  dialog.add_filter(filter)

  if dialog.run == Gtk::ResponseType::ACCEPT
    filename = dialog.filename
    file_content = File.read(filename)
    editor.buffer.text = file_content
  end
  dialog.destroy
end

def on_key_press(widget, event)
  ctrl_pressed = event.state.control_mask?
  s_pressed = event.keyval == Gdk::Keyval::KEY_S

  if ctrl_pressed && s_pressed
    puts "Ctrl-Sが押されました！"
    @drawing_area.signal_connect('draw') do |_, cr|
      width, height = parse_and_draw(editor, cr)
      @drawing_area.set_size_request(width + 30, height + 30)
      @drawing_area.queue_draw
    end
    return true # イベントの伝搬を止める
  end

  false # イベントの伝搬を許可
end

# カスタムアプリケーションのメインウィンドウクラス
class CustomApp < Gtk::Window
  def initialize
    super
    
    set_title('Custom Editor Application')
    set_default_size(1200, 900)
    signal_connect('delete-event') { Gtk.main_quit }

    # エディタのセットアップ
    editor_label = Gtk::Label.new("Editor")
    editor_label.set_markup("<b>Editor</b>")
    editor = Gtk::TextView.new
    editor.set_wrap_mode(Gtk::WrapMode::WORD)
    
    # スクロール可能なエディタを設定
    scrollable_editor = Gtk::ScrolledWindow.new
    scrollable_editor.add(editor)
    
    # セーブボタンのセットアップ
    save_button = Gtk::Button.new(:label => "Save")
    save_button.signal_connect('clicked') { save_json(editor) }

    # JSONファイルロードボタンのセットアップ
    load_json_button = Gtk::Button.new(:label => "Load JSON")
    load_json_button.signal_connect('clicked') { load_json(editor) }

    # CDLファイルロードボタンのセットアップ
    load_cdl_button = Gtk::Button.new(:label => "Load CDL")
    load_cdl_button.signal_connect('clicked') { load_cdl(editor) }

    # ボタンを格納するボックスを作成
    button_box = Gtk::Box.new(:horizontal, 5)
    button_box.pack_start(save_button, :expand => false, :fill => false, :padding => 5)
    button_box.pack_start(load_json_button, :expand => false, :fill => false, :padding => 5)
    button_box.pack_start(load_cdl_button, :expand => false, :fill => false, :padding => 5)

    # 右側のパネル（エディタ）
    right_panel = Gtk::Box.new(:vertical, 5)
    right_panel.pack_start(editor_label, :expand => false, :fill => false, :padding => 5)
    right_panel.pack_start(scrollable_editor, :expand => true, :fill => true, :padding => 5)
    right_panel.pack_start(button_box, :expand => false, :fill => false, :padding => 5)

    # 左側のパネルに描画領域を追加
    left_panel = Gtk::Box.new(:vertical, 5)

    # スクロール可能な描画領域を作成
    scrollable_drawing_area = Gtk::ScrolledWindow.new
    @drawing_area = Gtk::DrawingArea.new
    @drawing_area.set_size_request(8000, 6000)  # 描画領域のサイズを調整

    # 描画エリアをスクロール可能なウィンドウに追加
    scrollable_drawing_area.add_with_viewport(@drawing_area)
    scrollable_drawing_area.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC)

    # スクロール可能な描画エリアを左パネルに追加
    left_panel.pack_start(scrollable_drawing_area, :expand => true, :fill => true, :padding => 5)

    # エディタのバッファが変更されたときに描画を実行
    # editor.buffer.signal_connect('changed') do
    #   @drawing_area.signal_connect('draw') do |_, cr|
    #     width, height = parse_and_draw(editor, cr)
    #     @drawing_area.set_size_request(width + 30, height + 30)
    #     @drawing_area.queue_draw
    #   end
    # end

    editor.signal_connect('key_press_event') { |widget, event| on_key_press(widget, event) }

    # メインパネル（左右分割）
    hpaned = Gtk::Paned.new(:horizontal)
    hpaned.pack1(left_panel, resize: true, shrink: false)
    hpaned.pack2(right_panel, resize: true, shrink: false)

    # 左側のパネルの横幅を調整
    hpaned.position = 800  # この値を適切に設定

    # メインパネルをウィンドウに追加
    add(hpaned)

    show_all
  end
end

# アプリケーションの実行
app = CustomApp.new
Gtk.main