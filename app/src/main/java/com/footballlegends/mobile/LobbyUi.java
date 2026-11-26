package com.footballlegends.mobile;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.text.InputType;
import android.view.Gravity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;

/** Functional Android-native lobby navigation, local profile, squad and scene settings. */
public final class LobbyUi extends FrameLayout {
    private static final int PANEL = Color.argb(235, 9, 24, 45);
    private static final int ACCENT = Color.rgb(26, 190, 205);
    private final Context context;
    private final GameSurface gameSurface;
    private final LocalProfileStore profileStore;
    private final LinearLayout content;
    private final TextView pageTitle;
    private int formation = 0;

    private static final String[] FORMATIONS = {"4-3-3", "4-4-2", "3-5-2", "4-2-3-1"};
    private static final String[][] PLAYERS = {
            {"L. Costa","Goleiro","82"},{"R. Alves","Lateral D.","79"},
            {"M. Santos","Zagueiro","84"},{"D. Rocha","Zagueiro","81"},
            {"P. Lima","Lateral E.","80"},{"A. Nunes","Volante","83"},
            {"G. Ribeiro","Meia","86"},{"V. Martins","Meia","82"},
            {"T. Oliveira","Ponta D.","85"},{"C. Ferreira","Atacante","88"},
            {"I. Souza","Ponta E.","84"}
    };

    public LobbyUi(Context context, GameSurface surface) {
        super(context);
        this.context = context;
        this.gameSurface = surface;
        this.profileStore = new LocalProfileStore(context);
        setClipChildren(false);
        setClipToPadding(false);

        LinearLayout root = new LinearLayout(context);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(dp(18), dp(12), dp(18), dp(12));
        addView(root, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

        LinearLayout top = new LinearLayout(context);
        top.setGravity(Gravity.CENTER_VERTICAL);
        LinearLayout brandBox = new LinearLayout(context);
        brandBox.setOrientation(LinearLayout.VERTICAL);
        brandBox.addView(text("FOOTBALL LEGENDS", 19, Color.WHITE, true));
        brandBox.addView(text("MOBILE  •  CLUB HUB", 9, ACCENT, true));
        top.addView(brandBox, new LinearLayout.LayoutParams(0, LayoutParams.WRAP_CONTENT, 1));
        Button profile = button("PERFIL");
        profile.setOnClickListener(v -> showPage(3));
        top.addView(profile, new LinearLayout.LayoutParams(dp(104), dp(42)));
        root.addView(top, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(48)));

        LinearLayout middle = new LinearLayout(context);
        middle.setGravity(Gravity.CENTER_VERTICAL);
        root.addView(middle, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, 0, 1));
        LinearLayout panel = new LinearLayout(context);
        panel.setOrientation(LinearLayout.VERTICAL);
        panel.setPadding(dp(17), dp(14), dp(17), dp(14));
        panel.setBackground(round(PANEL, 18, Color.argb(90, 90, 180, 205)));
        middle.addView(panel, new LinearLayout.LayoutParams(dp(340), LayoutParams.MATCH_PARENT));
        pageTitle = text("CENTRAL DO CLUBE", 18, Color.WHITE, true);
        panel.addView(pageTitle);
        View divider = new View(context);
        divider.setBackgroundColor(ACCENT);
        panel.addView(divider, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(1)));
        ScrollView scroll = new ScrollView(context);
        scroll.setFillViewport(true);
        panel.addView(scroll, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, 0, 1));
        content = new LinearLayout(context);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setPadding(0, dp(10), 0, dp(6));
        scroll.addView(content);

        LinearLayout nav = new LinearLayout(context);
        nav.setGravity(Gravity.CENTER);
        nav.setPadding(dp(6), dp(5), dp(6), dp(5));
        nav.setBackground(round(Color.argb(225, 5, 14, 29), 16, Color.argb(70, 90, 170, 200)));
        String[] labels = {"INÍCIO", "MODOS", "ELENCO", "PERFIL", "AJUSTES"};
        for (int i = 0; i < labels.length; i++) {
            final int page = i;
            Button item = button(labels[i]);
            item.setTextSize(10);
            item.setOnClickListener(v -> showPage(page));
            nav.addView(item, new LinearLayout.LayoutParams(0, dp(43), 1));
        }
        root.addView(nav, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(54)));
        showPage(0);
    }

    private void showPage(int page) {
        content.removeAllViews();
        switch (page) {
            case 1: buildModes(); pageTitle.setText("MODOS DE JOGO"); break;
            case 2: buildSquad(); pageTitle.setText("ELENCO E TÁTICAS"); break;
            case 3: buildProfile(); pageTitle.setText("PERFIL LOCAL"); break;
            case 4: buildSettings(); pageTitle.setText("CONFIGURAÇÕES"); break;
            default: buildHome(); pageTitle.setText("CENTRAL DO CLUBE"); break;
        }
    }

    private void buildHome() {
        addText("BEM-VINDO AO SEU CLUBE", 14, ACCENT, true);
        addText("Seu estádio está pronto. Organize o time, ajuste a formação e prepare sua próxima temporada.", 13, Color.WHITE, false);
        addSpacer(14);
        addAction("ABRIR MODOS DE JOGO", () -> showPage(1));
        addAction("GERENCIAR ELENCO", () -> showPage(2));
        addAction("EDITAR PERFIL", () -> showPage(3));
        addSpacer(10);
        addText("PERFIL OFFLINE  •  DADOS SALVOS NESTE DISPOSITIVO", 9, Color.LTGRAY, true);
    }

    private void buildModes() {
        addText("ESCOLHA SUA PRÓXIMA EXPERIÊNCIA", 12, ACCENT, true);
        addMode("PARTIDA RÁPIDA", "Fluxo de partida será conectado ao motor nas fases de campo e gameplay.");
        addMode("CARREIRA", "Estrutura de carreira entra na fase de modos e progressão.");
        addMode("LIGAS E TORNEIOS", "Competições serão integradas quando o motor de partida estiver pronto.");
        addMode("TREINAMENTO", "Treinos dependem dos controles e da física de jogo.");
        addMode("ONLINE", "Serviços online dependem de backend real e serão ativados somente após testes.");
    }

    private void addMode(String title, String description) {
        LinearLayout card = card();
        card.addView(text(title, 12, Color.WHITE, true));
        card.addView(text(description, 10, Color.LTGRAY, false));
        card.addView(text("EM DESENVOLVIMENTO", 9, ACCENT, true));
        content.addView(card, params(-1, -2, 0, 0, 0, 8));
    }

    private void buildSquad() {
        addText("FORMAÇÃO TÁTICA", 11, ACCENT, true);
        Spinner selector = new Spinner(context);
        ArrayAdapter<String> adapter = new ArrayAdapter<>(context, android.R.layout.simple_spinner_dropdown_item, FORMATIONS);
        selector.setAdapter(adapter);
        formation = context.getSharedPreferences("football_legends_squad", Context.MODE_PRIVATE).getInt("formation", 0);
        selector.setSelection(Math.max(0, Math.min(formation, FORMATIONS.length - 1)));
        content.addView(selector, params(-1, 44, 0, 4, 0, 10));
        selector.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) {
                formation = position;
                context.getSharedPreferences("football_legends_squad", Context.MODE_PRIVATE)
                        .edit().putInt("formation", position).apply();
                gameSurface.setFormation(position);
            }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) { }
        });
        addText("TITULARES", 11, ACCENT, true);
        for (int i = 0; i < PLAYERS.length; i++) {
            String[] p = PLAYERS[i];
            LinearLayout row = new LinearLayout(context);
            row.setGravity(Gravity.CENTER_VERTICAL);
            row.setPadding(dp(9), dp(6), dp(9), dp(6));
            row.setBackground(round(Color.argb(145, 18, 38, 62), 9, Color.TRANSPARENT));
            row.addView(text(String.format("%02d", i + 1), 12, ACCENT, true),
                    new LinearLayout.LayoutParams(dp(30), LayoutParams.WRAP_CONTENT));
            LinearLayout nameBox = new LinearLayout(context);
            nameBox.setOrientation(LinearLayout.VERTICAL);
            nameBox.addView(text(p[0], 11, Color.WHITE, true));
            nameBox.addView(text(p[1], 9, Color.LTGRAY, false));
            row.addView(nameBox, new LinearLayout.LayoutParams(0, LayoutParams.WRAP_CONTENT, 1));
            row.addView(text(p[2], 13, Color.WHITE, true));
            content.addView(row, params(-1, 47, 0, 2, 0, 2));
        }
        addText("A formação escolhida altera a posição dos 11 modelos no estádio do lobby.", 9, Color.LTGRAY, false);
    }

    private void buildProfile() {
        addText("IDENTIDADE DO CLUBE", 11, ACCENT, true);
        addText(profileStore.getDisplayName(), 22, Color.WHITE, true);
        addText("Perfil local • Nível 1 • Progresso salvo neste dispositivo", 10, Color.LTGRAY, false);
        addSpacer(8);
        addAction("ALTERAR NOME", this::editProfileName);
        addText("Não há conta online ou sincronização simulada. A conexão com backend será implementada na fase online.", 10, Color.LTGRAY, false);
    }

    private void editProfileName() {
        EditText input = new EditText(context);
        input.setSingleLine(true);
        input.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_FLAG_CAP_WORDS);
        input.setText(profileStore.getDisplayName());
        new AlertDialog.Builder(context).setTitle("Nome do perfil")
                .setMessage("Escolha um nome de até 20 caracteres.")
                .setView(input).setNegativeButton("CANCELAR", null)
                .setPositiveButton("SALVAR", (dialog, which) -> {
                    if (profileStore.saveDisplayName(input.getText().toString())) showPage(3);
                    else new AlertDialog.Builder(context).setMessage("Nome inválido. Use de 1 a 20 caracteres.")
                            .setPositiveButton("OK", null).show();
                }).show();
    }

    private void buildSettings() {
        addText("CÂMERA DO ESTÁDIO", 11, ACCENT, true);
        Switch camera = new Switch(context);
        camera.setText("Movimento cinematográfico");
        camera.setTextColor(Color.WHITE);
        boolean cinematic = context.getSharedPreferences("football_legends_settings", Context.MODE_PRIVATE)
                .getBoolean("cinematic_camera", true);
        camera.setChecked(cinematic);
        camera.setOnCheckedChangeListener((button, checked) -> {
            context.getSharedPreferences("football_legends_settings", Context.MODE_PRIVATE)
                    .edit().putBoolean("cinematic_camera", checked).apply();
            gameSurface.setCinematicCamera(checked);
        });
        content.addView(camera, params(-1, 48, 0, 5, 0, 8));
        addText("A câmera e a formação são aplicadas diretamente à cena OpenGL. Opções de áudio e qualidade serão adicionadas quando seus sistemas estiverem integrados.", 10, Color.LTGRAY, false);
        addAction("RESTAURAR CÂMERA CINEMATOGRÁFICA", () -> camera.setChecked(true));
    }

    private void addText(String value, int size, int color, boolean bold) {
        TextView view = text(value, size, color, bold);
        view.setPadding(0, dp(5), 0, dp(5));
        content.addView(view, params(-1, -2, 0, 2, 0, 2));
    }

    private void addAction(String label, Runnable action) {
        Button button = button(label);
        button.setOnClickListener(v -> action.run());
        content.addView(button, params(-1, 45, 0, 5, 0, 5));
    }

    private void addSpacer(int height) {
        View spacer = new View(context);
        content.addView(spacer, params(1, height, 0, 0, 0, 0));
    }

    private LinearLayout card() {
        LinearLayout card = new LinearLayout(context);
        card.setOrientation(LinearLayout.VERTICAL);
        card.setPadding(dp(10), dp(8), dp(10), dp(8));
        card.setBackground(round(Color.argb(160, 18, 38, 62), 10, Color.TRANSPARENT));
        return card;
    }

    private TextView text(String value, int size, int color, boolean bold) {
        TextView view = new TextView(context);
        view.setText(value);
        view.setTextSize(size);
        view.setTextColor(color);
        if (bold) view.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        return view;
    }

    private Button button(String label) {
        Button button = new Button(context);
        button.setText(label);
        button.setTextSize(10);
        button.setTextColor(Color.WHITE);
        button.setAllCaps(false);
        button.setBackground(round(Color.argb(220, 13, 55, 78), 12, Color.argb(130, 26, 190, 205)));
        return button;
    }

    private GradientDrawable round(int color, int radius, int stroke) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(dp(radius));
        if (stroke != Color.TRANSPARENT) drawable.setStroke(dp(1), stroke);
        return drawable;
    }

    private LinearLayout.LayoutParams params(int width, int height, int left, int top, int right, int bottom) {
        LinearLayout.LayoutParams p = new LinearLayout.LayoutParams(
                width < 0 ? width : dp(width), height < 0 ? height : dp(height));
        p.setMargins(dp(left), dp(top), dp(right), dp(bottom));
        return p;
    }

    private int dp(int value) { return Math.round(value * getResources().getDisplayMetrics().density); }
}