require 'json'
require 'gtk3'
require 'rsvg2'
require 'ruby-graphviz'

$gen_base  = "gen"     # string: folder path to place generated files
$gen       = $gen_base # string: folder path to place generated files

def parse_and_draw(editor, cr)
  # エディタの内容を取得
  editor_content = editor.buffer.text

  # 内容を解析
  data = parse_input_from_string(editor_content)

  if data.nil?
    return 0, 0
  end

  # 解析結果を使用して描画
  width,height = draw_analysis(data, cr)

  return width, height
end

def parse_input_from_string(input_str)
  data = {}
  current_cls = nil
  current_som = nil

  begin
    # JSONファイルをパースしてハッシュに変換
    json_data = JSON.parse(input_str)
  rescue JSON::ParserError => e
    return nil
  end

  # data[:cycle_time]にあたる値をJSON中の"SystemCycleTime"の値にする
  data[:cycle_time] = json_data["SystemCycleTime"]

  # data[current_som.to_sym]にあたる値をJSON中の"Name"の値にする
  json_data["SOM"].each do |som|
    current_som = som["name"]
    data[current_som.to_sym] = {}

    all_flag = false
    is_first = true

    # data[current_som.to_sym][current_cls.to_sym]にあたる値をJSON中の"CLS_ALL_PRC1"や"CLS_PRC1"の値にする
    som.each do |key, value|
      # ここにCLS_ALL_PRCともう1つ以上クラス定義があった場合に描画更新を行わない
      if key.start_with?('CLS_')
        if !is_first && all_flag
          return nil
          # next
        end
        if key.start_with?('CLS_ALL_')
          all_flag = true
        end
        is_first = false
        current_cls = key
        data[current_som.to_sym][current_cls.to_sym] = {}

        # data[current_som.to_sym][current_cls.to_sym][dom.strip.to_sym]にあたる値をJSON中の"rUserDomain1"などの値にする
        value.each do |dom, val|
          # 数値が無効である場合、警告を出してスルーする
          if val.is_a?(String) && val.match?(/\A0+\z/)
            next
          end
          data[current_som.to_sym][current_cls.to_sym][dom.to_sym] = val
        end
      end
    end
  end

  data
end

def generate_automaton_diagram(json_data) 
  begin
    # JSONファイルをパースしてハッシュに変換
    soms = JSON.parse(json_data)['SOM']
  rescue JSON::ParserError => e
    graph = GraphViz.new(:G, type: :digraph)
    # 画像を生成
    svg_data = graph.output(:svg => String)
    return svg_data
  end

  # Graphvizオブジェクトを初期化（有向グラフ）
  graph = GraphViz.new(:G, type: :digraph)
  graph[:rankdir] = 'LR' # ノードを横一列に並べる

  # ノードを追加
  soms.each do |som|
    graph.add_nodes(som['name'])
  end

  # エッジを追加
  soms.each do |som|
    graph.add_edges(som['name'], som['next'], label: '') if som['next']
  end

  svg_data = graph.output(:svg => String)
  return svg_data
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

  # 例: フォントを設定
  font_options = Cairo::FontOptions.new
  font_options.antialias = Cairo::Antialias::DEFAULT

  cr.set_font_options(font_options)
  cr.select_font_face("Arial", Cairo::FontSlant::NORMAL, Cairo::FontWeight::NORMAL)
  cr.set_font_size(15)


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
      cls_count += 1
      data[som][cls].keys.each do |dom|
        unless dom_colors_hash.key?(dom)
          dom_colors_hash[dom] = colors[dom_size % colors.length]
          dom_size += 1
          # puts "Warning: dom_colors_hash[#{dom}] is nil. Set to default color." if dom_colors_hash[dom].nil?
          show_error_dialog("Warning: dom_colors_hash[#{dom}] is nil. Set to default color.") if dom_colors_hash[dom].nil?
        end
        # puts "Warning: dom_colors_hash[#{dom}] is nil. Set to default color." if dom_colors_hash[dom].nil?
        show_error_dialog("Warning: dom_colors_hash[#{dom}] is nil. Set to default color.") if dom_colors_hash[dom].nil?
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

  x_offset = sct_bs_s_x + 10
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
    left_som_x_line = x_offset + 15
    rigth_som_x_line = x_offset + sct_bs_x + 30
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
    cr.line_to(left_som_x_line, under_som_y_line + 10)
    cr.stroke
    # SOMの右縦線の描画
    cr.set_line_width(2)
    cr.move_to(rigth_som_x_line, 10)
    cr.line_to(rigth_som_x_line, under_som_y_line + 10)
    cr.stroke
    cr.set_dash([])

    width = rigth_som_x_line
    height = under_som_y_line


    # クラスごとのDOMの描画
    data[som].keys.each do |cls|
      # システムサイクルタイムの四角を描画
      cr.set_source_rgb(0, 0, 0)
      cr.rectangle(x_offset + 20, sct_bs_s_y + y_offset, sct_bs_x, sct_bs_y)
      cr.stroke
      # システムサイクルタイムのテキスト描画
      cr.move_to(x_offset + 320, sct_bs_s_y + y_offset + 20)
      cr.show_text("#{data[:cycle_time]}")
      # if cls == :CLS_ALL_PRC1
      if cls.to_s.start_with?("CLS_ALL_PRC")
        # クラス名のテキスト描画
        cr.move_to(x_offset - 70, (sct_bs_s_y + under_som_y_line)/2)
        cr.show_text("CLS_ALL")

        upper_dom_y_line = sct_bs_s_y + sct_bs_y
        under_dom_y_line = sct_bs_s_y + sct_bs_y + 100 * (cls_size-1) + line_length
        # DOMの左縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + 20, upper_dom_y_line)
        cr.line_to(x_offset + 20, under_dom_y_line)
        cr.stroke
        # DOMの右縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + sct_bs_x + 20, upper_dom_y_line)
        cr.line_to(x_offset + sct_bs_x + 20, under_dom_y_line)
        cr.stroke
        # DOMの下横線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + 20, under_dom_y_line)
        cr.line_to(x_offset + sct_bs_x + 20, under_dom_y_line)
        cr.stroke
            
        # クラスごとのDOMの描画
        x_dom_offset = x_offset
        data[som][cls].keys.each do |dom|
          x_ratio = data[som][cls][dom].to_f / data[:cycle_time].to_f
          x_add_offset = x_ratio * sct_bs_x

          # 背景色の四角形を描画
          # puts "Warning: dom_colors_hash[#{dom}] is nil. Set to default color." if dom_colors_hash[dom].nil?
          show_error_dialog("Warning: dom_colors_hash[#{dom}] is nil. Set to default color.") if dom_colors_hash[dom].nil?
          cr.set_source_rgb(dom_colors_hash[dom][0]/255.0, dom_colors_hash[dom][1]/255.0, dom_colors_hash[dom][2]/255.0)
          cr.rectangle(x_dom_offset + 21, sct_bs_s_y + sct_bs_y + 1 + y_offset, x_add_offset, under_dom_y_line - sct_bs_s_y - sct_bs_y - 2 )
          cr.fill

          # DOM名と時間のテキスト描画
          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10) + 20, sct_bs_s_y + y_offset + line_length + 8)
          cr.show_text("#{dom}")

          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10) + 20, sct_bs_s_y + y_offset + line_length + 23)
          cr.show_text("#{data[som][cls][dom]}")
                
          x_dom_offset += x_add_offset

          # 縦線の描画
          cr.set_line_width(2)
          cr.move_to(x_dom_offset + 20, sct_bs_s_y + sct_bs_y + y_offset)
          cr.line_to(x_dom_offset + 20, sct_bs_s_y + sct_bs_y + y_offset + under_dom_y_line - sct_bs_s_y - sct_bs_y)
          cr.stroke
        end # ALL CLS DOM END
      elsif 
        # クラス名のテキスト描画
        cr.move_to(x_offset - 70, sct_bs_s_y + sct_bs_y + y_offset + 10)
        cr.show_text("#{cls}")

        upper_dom_y_line = sct_bs_s_y + sct_bs_y + y_offset
        # DOMの左縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + 20, upper_dom_y_line)
        cr.line_to(x_offset + 20, upper_dom_y_line + line_length)
        cr.stroke
        # DOMの右縦線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + sct_bs_x + 20, upper_dom_y_line)
        cr.line_to(x_offset + sct_bs_x + 20, upper_dom_y_line + line_length)
        cr.stroke
        # DOMの下横線の描画
        cr.set_line_width(2)
        cr.move_to(x_offset + 20, upper_dom_y_line + line_length)
        cr.line_to(x_offset + sct_bs_x + 20, upper_dom_y_line + line_length)
        cr.stroke

        # クラスごとのDOMの描画
        x_dom_offset = x_offset
        data[som][cls].keys.each do |dom|
          x_ratio = data[som][cls][dom].to_f / data[:cycle_time].to_f
          x_add_offset = x_ratio * sct_bs_x

          # 背景色の四角形を描画
          cr.set_source_rgb(dom_colors_hash[dom][0]/255.0, dom_colors_hash[dom][1]/255.0, dom_colors_hash[dom][2]/255.0)
          cr.rectangle(x_dom_offset + 21, sct_bs_s_y + sct_bs_y + 1 + y_offset, x_add_offset, line_length - 2 )
          cr.fill

          # DOM名と時間のテキスト描画
          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10) + 20, sct_bs_s_y + y_offset + line_length + 8)
          cr.show_text("#{dom}")

          cr.set_source_rgb(0, 0, 0)
          cr.move_to((x_dom_offset + 10) + 20, sct_bs_s_y + y_offset + line_length + 23)
          cr.show_text("#{data[som][cls][dom]}")
          
          x_dom_offset += x_add_offset

          # 縦線の描画
          cr.set_line_width(2)
          cr.move_to(x_dom_offset + 20, sct_bs_s_y + sct_bs_y + y_offset)
          cr.line_to(x_dom_offset + 20, sct_bs_s_y + sct_bs_y + y_offset + line_length)
          cr.stroke
        end # CLS DOM END
      end # IF END
      x_dom_offset = x_offset
      y_offset += 100
    end # CLS END
    som_count += 1
    x_offset = sct_bs_s_x * (som_count + 1) + sct_bs_x * som_count + 20
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
  
  # JSON形式にそぐわないコードの場合にエラー吐かせる
  if dialog.run == Gtk::ResponseType::ACCEPT
    
    filename = dialog.filename
    File.open(filename, 'w') do |file|
      begin
        # JSONをパースしてハッシュに変換
        json_data = JSON.parse(editor.buffer.text)
        check_cls_all(json_data)
        generate_timewindow_cfg(filename, json_data)  # ハッシュを引数として渡す
        file.write(editor.buffer.text)
        rescue JSON::ParserError => e
          # JSON形式でない場合のエラーハンドリング
          show_error_dialog("Error: Invalid JSON format. cfg file is not generated. #{e.message}")
        rescue => e
          # その他のエラー
          show_error_dialog("Error: #{e.message}")
      end
    end
  end
  dialog.destroy
end

def check_cls_all(json_data)
  if json_data['SOM']
    json_data['SOM'].each do |som_item|
      cls_all_prc_keys = som_item.keys.select { |k| k.start_with?('CLS_ALL_PRC') }
      has_cls_prc = som_item.keys.any? { |k| k.start_with?('CLS_PRC') && !k.start_with?('CLS_ALL_PRC') }

      if cls_all_prc_keys.count >= 2 || (cls_all_prc_keys.any? && has_cls_prc)
        raise 'Multiple CLS_ALL_PRC keys or CLS_ALL_PRC and CLS_PRC keys cannot coexist in the same SOM element.'
      end
    end
  end
end

def show_error_dialog(message)
  # エラーメッセージを表示するダイアログの作成
  error_dialog = Gtk::MessageDialog.new(
    message: message,
    type: :error,
    buttons: :ok
  )
  error_dialog.run
  error_dialog.destroy
end

def generate_timewindow_cfg(output_filename, json_data)
  # エラー
  File.open("gen/Timewindow.cfg", 'w') do |file|
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

    # ボタンを格納するボックスを作成
    button_box = Gtk::Box.new(:horizontal, 5)
    button_box.pack_start(save_button, :expand => false, :fill => false, :padding => 5)
    button_box.pack_start(load_json_button, :expand => false, :fill => false, :padding => 5)
    # button_box.pack_start(load_cdl_button, :expand => false, :fill => false, :padding => 5)

    # 右側のパネル（エディタ）
    right_panel = Gtk::Box.new(:vertical, 5)
    right_panel.pack_start(editor_label, :expand => false, :fill => false, :padding => 5)
    right_panel.pack_start(scrollable_editor, :expand => true, :fill => true, :padding => 5)
    right_panel.pack_start(button_box, :expand => false, :fill => false, :padding => 5)

    # 左側のパネルに描画領域を追加
    left_panel = Gtk::Paned.new(:vertical)

    # スクロール可能な描画領域を作成
    scrollable_drawing_area = Gtk::ScrolledWindow.new
    @drawing_area = Gtk::DrawingArea.new
    @drawing_area.set_size_request(8000, 6000)  # 描画領域のサイズを調整

    # 描画エリアをスクロール可能なウィンドウに追加
    scrollable_drawing_area.add_with_viewport(@drawing_area)
    scrollable_drawing_area.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC)

    # スクロール可能な描画エリアを左パネルに追加
    upper_area = Gtk::Box.new(:horizontal, 5)
    lower_area = Gtk::Box.new(:horizontal, 5)

    # 上側に何かを追加する場合はここにコードを追加
    upper_area.pack_start(scrollable_drawing_area, :expand => true, :fill => true, :padding => 5)

    # 下側に画像を追加する
    img = generate_automaton_diagram(editor.buffer.text)

    @drawing_automaton_area = Gtk::DrawingArea.new

    lower_area.pack_start(@drawing_automaton_area, :expand => true, :fill => true, :padding => 5)

    left_panel.pack1(upper_area, :expand => true, :fill => true, :padding => 5)
    left_panel.pack2(lower_area, :expand => true, :fill => true, :padding => 5)

    left_panel.position = 650  # この値を適切に設定

    # 描画する内容を管理する変数を初期化
    @should_draw = true

    @drawing_area.signal_connect('draw') do |_, cr|
      if @should_draw
        width, height = parse_and_draw(editor, cr)
        @drawing_area.set_size_request(width + 30, height + 30)
      end
    end

    @drawing_automaton_area.signal_connect('draw') do |_, cr|
      if @should_draw
        editor_content = editor.buffer.text
        img = generate_automaton_diagram(editor_content)
        svg = RSVG::Handle.new_from_data(img)
        
        # SVGの元のサイズを取得
        svg_width = svg.width
        svg_height = svg.height

        # DrawingAreaのサイズを取得
        allocation = @drawing_automaton_area.allocation
        widget_width = allocation.width
        widget_height = allocation.height

        # スケーリング係数を計算（アスペクト比を維持）
        scale_x = widget_width.to_f / svg_width
        scale_y = widget_height.to_f / svg_height
        scale = [scale_x, scale_y].min * 0.9  # アスペクト比を維持するために小さい方を選択

        # 中央に移動
        cr.translate((widget_width - svg_width * scale) / 2, (widget_height - svg_height * scale) / 2) 

        # スケーリングを適用
        cr.scale(scale, scale)

        # SVGをレンダリング
        cr.render_rsvg_handle(svg)
      end
    end

    editor.buffer.signal_connect('changed') do
      # width と height をチェックして @should_draw を更新
      editor_content = editor.buffer.text
      # 内容を解析
      data = parse_input_from_string(editor_content)
      img = generate_automaton_diagram(editor_content)
      @should_draw = data != nil

      if @should_draw
        @drawing_area.queue_draw
        @drawing_automaton_area.queue_draw
      end
    end

    # メインパネル（左右分割）
    hpaned = Gtk::Paned.new(:horizontal)
    hpaned.pack1(left_panel, resize: true, shrink: false)
    hpaned.pack2(right_panel, resize: true, shrink: false)

    # 左側のパネルの横幅を調整
    hpaned.position = 950  # この値を適切に設定

    # メインパネルをウィンドウに追加
    add(hpaned)

    show_all
  end
end

# アプリケーションの実行
app = CustomApp.new
Gtk.main
