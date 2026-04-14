import streamlit as st
import pandas as pd
import os
import re
import matplotlib.pyplot as plt

#чистка
def clean_vhi_file(filepath, province_id):
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()

    data_lines = [line for line in lines if re.match(r"^\d", line)]

    rows = [line.strip().split(",")[:7] for line in data_lines]

    df = pd.DataFrame(
        rows,
        columns=["Year", "Week", "SMN", "SMT", "VCI", "TCI", "VHI"]
    )

    df = df.apply(pd.to_numeric, errors="coerce")
    df = df.dropna()

    df["NOAA_ID"] = province_id

    return df


#завантаження
@st.cache_data
def load_data():
    all_data = []

    for file in os.listdir("data"):
        if file.endswith(".csv"):
            province_id = int(file.split("_")[1])
            path = os.path.join("data", file)

            df = clean_vhi_file(path, province_id)
            all_data.append(df)

    df = pd.concat(all_data, ignore_index=True)

    return df


df = load_data()


#зміна назв
ukrainian_regions = [
    "Вінницька","Волинська","Дніпропетровська","Донецька","Житомирська",
    "Закарпатська","Запорізька","Івано-Франківська","Київська","Кіровоградська",
    "Крим","Луганська","Львівська","Миколаївська","Одеська","Полтавська",
    "Рівненська","Сумська","Тернопільська","Харківська","Херсонська",
    "Хмельницька","Черкаська","Чернівецька","Чернігівська"
]

ua_index = {name: i + 1 for i, name in enumerate(ukrainian_regions)}

noaa_to_name = {
    1:"Черкаська",2:"Чернігівська",3:"Чернівецька",4:"Крим",
    5:"Дніпропетровська",6:"Донецька",7:"Івано-Франківська",8:"Харківська",
    9:"Херсонська",10:"Хмельницька",11:"Київська",12:"Кіровоградська",
    13:"Луганська",14:"Львівська",15:"Миколаївська",16:"Одеська",
    17:"Полтавська",18:"Рівненська",19:"Сумська",20:"Тернопільська",
    21:"Закарпатська",22:"Вінницька",23:"Волинська",24:"Житомирська",
    25:"Запорізька"
}

df["Province_Name"] = df["NOAA_ID"].map(noaa_to_name)
df["UA_ID"] = df["Province_Name"].map(ua_index)


#мінімум і максимум
min_year, max_year = int(df["Year"].min()), int(df["Year"].max())
min_week, max_week = int(df["Week"].min()), int(df["Week"].max())

#layout
col1, col2 = st.columns([1, 3])

#фільтри
with col1:
    st.title("Фільтри")

    index_type = st.selectbox("Індекс", ["VCI", "TCI", "VHI"])
    region = st.selectbox("Область", df["Province_Name"].dropna().unique())

    year_range = st.slider("Роки", min_year, max_year, (min_year, max_year))
    week_range = st.slider("Тижні", min_week, max_week, (min_week, max_week))

    if st.button("Скинути фільтри"):
        st.session_state.clear()
        st.rerun()

    sort_asc = st.checkbox("Сортування ↑")
    sort_desc = st.checkbox("Сортування ↓")

    if sort_asc and sort_desc:
        st.warning("Можна вибрати тільки один тип сортування")
        sort_asc = sort_desc = False

#фільтрація
filtered = df[
    (df["Province_Name"] == region) &
    (df["Year"].between(year_range[0], year_range[1])) &
    (df["Week"].between(week_range[0], week_range[1]))
]

if sort_asc:
    filtered = filtered.sort_values(index_type, ascending=True)
elif sort_desc:
    filtered = filtered.sort_values(index_type, ascending=False)

#tabs
with col2:
    tab1, tab2, tab3 = st.tabs(["Таблиця", "Графік", "Порівняння"])

    #таблиця
    with tab1:
        st.dataframe(filtered)

    #граф1
    with tab2:
        fig, ax = plt.subplots()
        ax.plot(filtered["Year"] + filtered["Week"] / 52, filtered[index_type])
        ax.set_title(f"{index_type} — {region}")
        ax.set_xlabel("Time (Year)")
        ax.set_ylabel(index_type)
        st.pyplot(fig)

    #граф2
    with tab3:
        compare = df[
            (df["Year"].between(year_range[0], year_range[1])) &
            (df["Week"].between(week_range[0], week_range[1]))
        ]

        fig2, ax2 = plt.subplots()

        for reg in compare["Province_Name"].dropna().unique():
            temp = compare[compare["Province_Name"] == reg]

            ax2.plot(
                temp["Year"] + temp["Week"] / 52,
                temp[index_type],
                alpha=0.4 if reg != region else 1.0,
                linewidth=2 if reg == region else 1
            )

        ax2.set_title(f"Порівняння {index_type}")
        ax2.set_xlabel("Time (Year)")
        ax2.set_ylabel(index_type)

        st.pyplot(fig2)